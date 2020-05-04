#include "arc.h"
#include <cmath>
#include <QDebug>

double determinant(QPoint a, QPoint b, QPoint c) {
    return a.x()*b.y() - a.x()*c.y() - a.y()*b.x() + a.y()*c.x() + b.x()*c.y() - b.y()*c.x();
}

Arc::Arc()
{
}

Arc::Arc(QPoint p1, QPoint p2, QPoint p3)
{
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    radius = sqrt((p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y()));
}

void Arc::addPoint(QPoint p)
{
    points.push_back(p);

    if (this->points.size() == 2) {
       Line newLine = Line(QPoint(points[points.size()-2].x(), points[points.size()-2].y()), QPoint(p.x(), p.y()));
       lines.push_back(newLine);
   }

    if (this->points.size() == 3) {
       Line newLine = Line(QPoint(points[points.size()-3].x(), points[points.size()-3].y()), QPoint(p.x(), p.y()));
       lines.push_back(newLine);
   }
}

QPoint Arc::getPoint(int i)
{
    return points[i];
}

std::vector<Pixel> Arc::getPixels()
{
    std::vector<Pixel> pixels;

    int x1 = points[0].x();
    int y1 = points[0].y();

    int det = determinant(points[0], points[1], points[2]);
    bool case1 = (det > 0);
    bool case2 = (det < 0);

//  draws lines here
    for (auto line: lines) {
        line.setThickness(thickness);
        std::vector<Pixel> newPixels = line.getPixels();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    int radius = sqrt(pow(points[1].x() - points[0].x(), 2) + pow(points[1].y() - points[0].y(), 2) * 1.0);

//  circle part
    int dE = 3;
    int dSE = 5 - 2 * radius;
    int d = 1 - radius;
    int x = radius;
    int y = 0;

    if (case1 && determinant(points[0], points[1], QPoint(x1, y1+radius)) > 0 && determinant(points[0], points[2], QPoint(x1, y1+radius)) < 0)
        pixels.push_back(Pixel(x1, y1+radius));
    else if (case2 && (determinant(points[0], points[1], QPoint(x1, y1+radius)) > 0 || determinant(points[0], points[2], QPoint(x1, y1+radius)) < 0))
        pixels.push_back(Pixel(x1, y1+radius));

    if (case1 && determinant(points[0], points[1], QPoint(x1, y1-radius)) > 0 && determinant(points[0], points[2], QPoint(x1, y1-radius)) < 0)
        pixels.push_back(Pixel(x1, y1-radius));
    else if (case2 && (determinant(points[0], points[1], QPoint(x1, y1-radius)) > 0 || determinant(points[0], points[2], QPoint(x1, y1-radius)) < 0))
        pixels.push_back(Pixel(x1, y1-radius));

    if (case1 && determinant(points[0], points[1], QPoint(x1+radius, y1)) > 0 && determinant(points[0], points[2], QPoint(x1+radius, y1)) < 0)
        pixels.push_back(Pixel(x1+radius, y1));
    else if (case2 && (determinant(points[0], points[1], QPoint(x1+radius, y1)) > 0 || determinant(points[0], points[2], QPoint(x1+radius, y1)) < 0))
        pixels.push_back(Pixel(x1+radius, y1));

    if (case1 && determinant(points[0], points[1], QPoint(x1-radius, y1)) > 0 && determinant(points[0], points[2], QPoint(x1-radius, y1)) < 0)
        pixels.push_back(Pixel(x1-radius, y1));
    else if (case2 && (determinant(points[0], points[1], QPoint(x1-radius, y1)) > 0 || determinant(points[0], points[2], QPoint(x1-radius, y1)) < 0))
        pixels.push_back(Pixel(x1-radius, y1));

    while (x > y)
    {
        y++;
        if (d <= 0) {
            d += dE;
            dE += 2;
            dSE += 2;
        }
        else {
            d += dSE;
            dE += 2;
            dSE += 4;
            x--;
        }

        if (x < y) break;

        if (case1 && determinant(points[0], points[1], QPoint(x1+x, y1+y)) > 0 && determinant(points[0], points[2], QPoint(x+x1, y+y1)) < 0)
            pixels.push_back(Pixel(x1+x,y1+y));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1+x, y1+y)) > 0 || determinant(points[0], points[2], QPoint(x1+x, y1+y)) < 0))
            pixels.push_back(Pixel(x1+x,y1+y));

        if (case1 && determinant(points[0], points[1], QPoint(x1-x,y1+y)) > 0 && determinant(points[0], points[2], QPoint(x1-x,y1+y)) < 0)
            pixels.push_back(Pixel(x1-x,y1+y));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1-x,y1+y)) > 0 || determinant(points[0], points[2], QPoint(x1-x,y1+y)) < 0))
            pixels.push_back(Pixel(x1-x,y1+y));

        if (case1 && determinant(points[0], points[1], QPoint(x1+x,y1-y)) > 0 && determinant(points[0], points[2], QPoint(x1+x,y1-y)) < 0)
            pixels.push_back(Pixel(x1+x,y1-y));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1+x,y1-y)) > 0 || determinant(points[0], points[2], QPoint(x1+x,y1-y)) < 0))
            pixels.push_back(Pixel(x1+x,y1-y));

        if (case1 && determinant(points[0], points[1], QPoint(x1-x,y1-y)) > 0 && determinant(points[0], points[2], QPoint(x1-x,y1-y)) < 0)
            pixels.push_back(Pixel(x1-x,y1-y));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1-x,y1-y)) > 0 || determinant(points[0], points[2], QPoint(x1-x,y1-y)) < 0))
            pixels.push_back(Pixel(x1-x,y1-y));

        if (case1 && determinant(points[0], points[1], QPoint(x1+y,y1+x)) > 0 && determinant(points[0], points[2], QPoint(x1+y,y1+x)) < 0)
            pixels.push_back(Pixel(x1+y,y1+x));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1+y,y1+x)) > 0 || determinant(points[0], points[2], QPoint(x1+y,y1+x)) < 0))
            pixels.push_back(Pixel(x1+y,y1+x));

        if (case1 && determinant(points[0], points[1], QPoint(x1+y,y1-x)) > 0 && determinant(points[0], points[2], QPoint(x1+y,y1-x)) < 0)
            pixels.push_back(Pixel(x1+y,y1-x));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1+y,y1-x)) > 0 || determinant(points[0], points[2], QPoint(x1+y,y1-x)) < 0))
            pixels.push_back(Pixel(x1+y,y1-x));

        if (case1 && determinant(points[0], points[1], QPoint(x1-y,y1+x)) > 0 && determinant(points[0], points[2], QPoint(x1-y,y1+x)) < 0)
            pixels.push_back(Pixel(x1-y,y1+x));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1-y,y1+x)) > 0 || determinant(points[0], points[2], QPoint(x1-y,y1+x)) < 0))
            pixels.push_back(Pixel(x1-y,y1+x));

        if (case1 && determinant(points[0], points[1], QPoint(x1-y,y1-x)) > 0 && determinant(points[0], points[2], QPoint(x1-y,y1-x)) < 0)
            pixels.push_back(Pixel(x1-y,y1-x));
        else if (case2 && (determinant(points[0], points[1], QPoint(x1-y,y1-x)) > 0 || determinant(points[0], points[2], QPoint(x1-y,y1-x)) < 0))
            pixels.push_back(Pixel(x1-y,y1-x));
    }

    return pixels;

}

std::vector<Pixel> Arc::getPixelsAA()
{
    std::vector<Pixel> pixels;
    return pixels;
}

json Arc::getJsonFormat()
{
    json item;
    item["shape"] = "arc";

    return item;
}
