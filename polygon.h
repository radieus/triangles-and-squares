#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include "line.h"

class Polygon : public Shape
{
public:
    Polygon();
    Polygon(std::vector<QPoint>);

    void addPoint(QPoint p) override;
    QPoint getPoint(int i) override;

    std::vector<Pixel> getPixels() override;

    int getPointsSize() { return points.size(); }

    std::vector<QPoint> points;
    std::vector<Line> lines;
};

#endif // POLYGON_H
