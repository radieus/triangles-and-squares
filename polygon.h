#ifndef POLYGON_H
#define POLYGON_H

#include <QPixmap>
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

    std::vector<Pixel> Clip(Polygon clipper);

    json getJsonFormat() override;

    std::vector<PixelWithColor> getFillingPixels();
    void setFillColor(Color color);
    void setFillColor(QImage image);
    bool isFilled = false;

    QImage fillingImage;
    uchar* bits;

    PixelWithColor getPixelFromImage(int x, int y);

    bool isFilledWithImage = false;
    bool isFilledWithColor = false;

    Color fillCol = {0, 0, 0};
    int yMinTemp;

};

#endif // POLYGON_H
