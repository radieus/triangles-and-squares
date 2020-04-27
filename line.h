#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class Line : public Shape
{
public:
    Line();
    Line(QPoint p1, QPoint p2);

    void addPoint(QPoint p) override;
    QPoint getPoint(int i) override;

    std::vector<Pixel> getPixels() override;

    json getJsonFormat() override;

};

#endif // LINE_H
