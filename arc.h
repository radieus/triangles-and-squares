#ifndef ARC_H
#define ARC_H

#include <QPoint>
#include "Shape.h"
#include "line.h"

class Arc : public Shape
{
public:
    Arc();
    Arc(QPoint p1, QPoint p2, QPoint p3);

    void addPoint(QPoint p) override;
    QPoint getPoint(int i) override;

    std::vector<Pixel> getPixels() override;

    std::vector<QPoint> points;
    std::vector<Line> lines;

private:
    int radius;

};

#endif // ARC_H
