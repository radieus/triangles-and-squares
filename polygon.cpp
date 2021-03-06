#include "polygon.h"
#include "aet.h"
#include <QDebug>
#include <list>

bool CompareByXmin(const AETNode& a, const AETNode& b){ return a.xMin < b.xMin; }

bool CompareByY(const QPoint& a, const QPoint& b){ return a.y() < b.y(); }

bool CompareInPair(const std::pair<int, int>& el1, const std::pair<int, int>& el2) {return el1.first < el2.first;}

double dotProduct(QPoint a, QPoint b) {
    return a.x() * b.x() + a.y() * b.y();
}

QPoint parametrized(double t, QPoint a, QPoint b) {
    return QPoint(a - (a - b) * t);
}

void Polygon::setFillColor(Color color)
{
     fillCol = color;
     isFilled = true;
     isFilledWithColor = true;
     isFilledWithImage = false;
}

void Polygon::setFillColor(QImage image)
{
    isFilled = true;
    isFilledWithColor = false;
    isFilledWithImage = true;
    fillingImage = image.convertToFormat(QImage::Format_BGR888);
    bits = fillingImage.bits();
}

QString Polygon::getPathToImage() const
{
    return pathToImage;
}

void Polygon::setPathToImage(const QString &value)
{
    pathToImage = value;
}

PixelWithColor Polygon::getPixelFromImage(int x, int y) {
    int newR, newB, newG;
    int xx = x % fillingImage.width();
    int yy = y % fillingImage.height();

    int pos = xx*3 + yy*fillingImage.width()*3;
    newB = *(bits + pos);
    newG = *(bits + pos + 1);
    newR = *(bits + pos + 2);
    return PixelWithColor(x, y, newR, newB, newG);
}

Polygon::Polygon()
{
}

Polygon::Polygon(std::vector<QPoint> points)
{
    for (QPoint point : points)
        addPoint(point);
}

void Polygon::addPoint(QPoint p)
{
    points.push_back(p);
    if (this->points.size() > 1) {
       Line newLine = Line(QPoint(points[points.size()-2].x(), points[points.size()-2].y()), QPoint(p.x(), p.y()));
       lines.push_back(newLine);
   }
}

QPoint Polygon::getPoint(int i)
{
    return points[i];
}

std::vector<Pixel> Polygon::getPixels()
{
    std::vector<Pixel> pixels;

    Line newLine = Line(QPoint(points[points.size()-1].x(), points[points.size()-1].y()), QPoint(lines[0].getPoint(0).x(), lines[0].getPoint(0).y()));
    lines.push_back(newLine);

    for (auto line: lines) {
        line.setThickness(thickness);
        std::vector<Pixel> newPixels = line.getPixels();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    return pixels;
}

std::vector<Pixel> Polygon::getPixelsAA()
{
    std::vector<Pixel> pixels;

    Line newLine = Line(QPoint(points[points.size()-1].x(), points[points.size()-1].y()), QPoint(lines[0].getPoint(0).x(), lines[0].getPoint(0).y()));
    lines.push_back(newLine);

    for (auto line: lines) {
        line.setThickness(thickness);
        std::vector<Pixel> newPixels = line.getPixelsAA();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    return pixels;
}

std::vector<Pixel> Polygon::Clip(Polygon clipper)
{
    std::vector<Pixel> pixels;
    double tE;
    double tL;
    qDebug() << "dzien dobry";

    //foreach line to be clipped
    for (int i = 0; i < getPointsSize(); ++i){
        tE = 0;
        tL = 1;
        QPoint p0 = this->getPoint(i);
        QPoint p1 = this->getPoint((i + 1) % this->getPoints().size());
        QPoint D(p1.x() - p0.x(), p1.y() - p0.y());

        //foreach clip edge
        for (int j = 0; j < clipper.getPointsSize(); ++j){
            QPoint a = clipper.getPoint(i);
            QPoint b = clipper.getPoint((i + 1) % clipper.getPoints().size());

            //precalculating Ni and choose PEi for each edge
            QPoint PE((a.x() + b.x())/2, (a.y() + b.y())/2);
            QPoint N(b.y() - a.y(), b.x() - a.x());
            double dp = dotProduct(N, D);
            if (dp == 0) {
                if (dotProduct(N, p0 - PE) > 0)
                    continue; //discord
            }
            else {
                double t = dotProduct(N, p0 - PE) / dotProduct(N * -1, D);
                if (dp < 0)
                    tE = std::max(tE, t);
                else if (dp > 0)
                    tL = std::min(tL, t);
                else continue;
            }
        }

        if (tE > tL)
            continue;

        Line tmpLine(QPoint(parametrized(tE, p0, p1).x(), parametrized(tE, p0, p1).y()),
                     QPoint(parametrized(tL, p0, p1).x(), parametrized(tL, p0, p1).y()));

        qDebug() << "x0:" << tmpLine.getPoint(0).x() << "y0" << tmpLine.getPoint(0).y() << "x1" << tmpLine.getPoint(1).x() << "y1" << tmpLine.getPoint(1).y();

        pixels.insert(pixels.begin(), tmpLine.getPixels().begin(), tmpLine.getPixels().end());
    }

    qDebug() << "do widzenia";

    return pixels;
}

//    //precalculate Ni and choose PEi for each edge
//    foreach line to be clipped {
//        if (P0 == P1)
//            line degenerated so clip as point;
//        else {
//            tE = 0, tL = 1;
//            foreach clip edge {
//                if (Ni*D == 0) { // parallel lines
//                    if (N*(P0-PEi) > 0) { // outside of parallel edge
//                        discard;
//                        return;
//                    }
//                } else {
//                    calculate t; //of line and edge intersection
//                    use signe of Ni*D to categorize as PE or PL;
//                    if (PE)
//                    tE = max(tE, t);
//                    else //PL
//                    tL = min(tL, t);
//                }
//            }
//            if (tE > tL)
//            discard;
//            else
//            draw line from P(tE) to P(tL);
//        }
//    }

std::vector<PixelWithColor> Polygon::getFillingPixels()
{
    std::vector<PixelWithColor> pixels;
    std::vector<int> indices;
    std::vector<std::pair<int, int> > tmp;
    AET aet = AET();
    int fillR = fillCol.r;
    int fillG = fillCol.g;
    int fillB = fillCol.b;

    // create indices
    int i = 0;
    for (auto point: points)
        tmp.push_back(std::make_pair(point.y(), i++));

    std::sort(tmp.begin(), tmp.end(), CompareInPair);

    for (auto p: tmp)
        indices.push_back(p.second);

    int k = 0;
    i = indices[k];
    int N = points.size();
    int y, yMin, yMax;
    y = yMin = points[indices[0]].y();
    yMax = points[indices[N-1]].y();

    while (y < yMax) {
        while (points[i].y() == y) {

            // wrapping indices
            int prev = i-1, next = i + 1;
            if (prev == -1)
                prev = N-1;
            if (next == N)
                next = 0;

            if (points[prev].y() > points[i].y())
                aet.Add(AETNode(QPoint(points[i].x(), points[i].y()), QPoint(points[prev].x(), points[prev].y())));
            if (points[next].y() > points[i].y())
                aet.Add(AETNode(QPoint(points[i].x(), points[i].y()), QPoint(points[next].x(), points[next].y())));
            ++k;
            i = indices[k];
        }

        //sort AET by x value
        aet.nodes.sort(CompareByXmin);

        //fill pixels between pairs of intersections
        int bucketSize = aet.nodes.size();
        std::vector<double> xVals;
        std::list<AETNode>::iterator it;
        for (it = aet.nodes.begin(); it != aet.nodes.end(); ++it)
            xVals.push_back(it->xMin);

        for (int k = 0; k < bucketSize - 1; k+=2) {
            int beginX = ceil(xVals[k]);
            int endX = floor(xVals[k + 1]);
            for (int i = beginX; i <= endX; i++)
                if (isFilledWithColor)
                    pixels.push_back(PixelWithColor(i, y, fillR, fillG, fillB));
                else
                    pixels.push_back(getPixelFromImage(i, y));

        }

        y++;

        //remove from AET edges for which ymax = y
        aet.nodes.remove_if([y](AETNode &a){return a.yMax == y;});
        for (auto &edge: aet.nodes)
            edge.xMin += edge.m_inverse;
    }

    return pixels;
}

json Polygon::getJsonFormat()
{
    json item;
    item["shape"] = "polygon";
    for (auto point : getPoints()) {
        item["points"].push_back({point.x(), point.y()});
    }
    item["color"] = {getColor().r, getColor().g, getColor().b};
    item["thickness"] = getThickness();
    if (isFilledWithColor)
        item["fillcolor"] = {fillCol.r, fillCol.g, fillCol.b};
    if (isFilledWithImage)
        item["fillimage"] = getPathToImage().toUtf8();

    return item;
}

