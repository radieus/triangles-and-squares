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
    std::vector<Pixel> getPixelsAA() override;

    int getPointsSize() { return points.size(); }

    std::vector<Line> lines;

    json getJsonFormat() override;
};

#endif // POLYGON_H
