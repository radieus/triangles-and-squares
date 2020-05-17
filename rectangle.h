#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QPoint>
#include "Shape.h"
#include "line.h"

class Rectangle : public Shape
{
public:
    Rectangle();
    Rectangle(QPoint p1, QPoint p2);

    void addPoint(QPoint p) override;
    QPoint getPoint(int i) override;

    std::vector<Pixel> getPixels() override;
    std::vector<Pixel> getPixelsAA() override;

    int getPointsSize() { return points.size(); }

    std::vector<Line> lines;

    json getJsonFormat() override;
};

#endif // RECTANGLE_H
