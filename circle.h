#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPoint>
#include "Shape.h"

class Circle : public Shape
{
public:
    Circle();
    Circle(QPoint p1, QPoint p2);

    void addPoint(QPoint p) override;
    QPoint getPoint(int i) override;

    std::vector<Pixel> getPixels() override;
    std::vector<Pixel> getPixelsAA() override;

    int getRadius() {return radius;}

    json getJsonFormat() override;

private:
    int radius;

};

#endif // CIRCLE_H
