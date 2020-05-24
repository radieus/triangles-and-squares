#include "polygon.h"
#include "aet.h"
#include <QDebug>
#include <list>

bool CompareByYmin(const AETNode& a, const AETNode& b){ return a.yMin < b.yMin; }

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

void Polygon::Clip(Polygon clipper)
{
    std::vector<Pixel> pixels;
    Polygon clippedShape;
    double tE;
    double tL;

    for (auto &line : lines){
        tE = 0;
        tL = 1;
        QPoint p0 = line.getPoint(0);
        QPoint p1 = line.getPoint(1);
        QPoint D(p1.x() - p0.x(), p1.y() - p0.y());

        for (auto &clipEdge : clipper.lines){
            QPoint a = clipEdge.getPoint(0);
            QPoint b = clipEdge.getPoint(1);
            QPoint PE((a.x() + b.x())/2, (a.y() + b.y())/2);
            QPoint N(b.y() - a.y(), b.x() - a.x());

            double dp = dotProduct(N, D);

            double t = dotProduct(N, p0 - PE) / dotProduct(N * -1, D);
            if (dp < 0)
                tE = std::max(tE, t);
            else if (dp > 0)
                tL = std::min(tL, t);
            else continue;
        }
        if (tE > tL)
            continue;

        Line tmpLine(QPoint(parametrized(tE, p0, p1).x(), parametrized(tE, p0, p1).y()),
                     QPoint(parametrized(tL, p0, p1).x(), parametrized(tL, p0, p1).y()));

        pixels.insert(pixels.begin(), tmpLine.getPixels().begin(), tmpLine.getPixels().end());
    }

}

//    //precalculate Ni and choose PEi for each edge
//    foreach line to be clipped {
//        if (P0 == P1)
//            line degenerated so clip as point;
//        else {
//            tE = 0, tL = 1;
//            foreach clip edge {
//                if (Ni*D == 0) { // parallel lines
//                if (N*(P0-PEi) > 0) { // outside of parallel edge
//                    discard;
//                    return;
//                }
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

std::vector<Pixel> Polygon::getFillingPixels()
{
    std::vector<Pixel> pixels;
    std::vector<int> indices;
    std::vector<std::pair<int, int> > tmp;
    AET aet = AET();

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
        aet.nodes.sort( [](const AETNode& a, const AETNode& b) {
                return a.xMin < b.xMin; });

        // fill pixels between pairs of intersections
        int bucketSize = aet.nodes.size();
        std::vector<double> xVals;
        std::list<AETNode>::iterator it;
        for (it = aet.nodes.begin(); it != aet.nodes.end(); ++it)
            xVals.push_back(it->xMin);

        for (int k = 0; k < bucketSize - 1; k+=2) {
            int beginX = ceil(xVals[k]);
            int endX = floor(xVals[k + 1]);
            for (int i = beginX; i <= endX; i++)
                pixels.push_back(Pixel(i, y));
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

    return item;
}

