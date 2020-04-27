#include "polygon.h"

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


