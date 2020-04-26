#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <vector>
#include "color.h"
#include "pixel.h"

class Shape {
    public:
        virtual void addPoint(QPoint p) = 0;
        virtual QPoint getPoint(int i) = 0;

        virtual std::vector<Pixel> getPixels() = 0;

        Color getColor() {return color;}
        void setColor(Color color) {this->color = color;}

        void setThickness(int t) {thickness = t;}
        int getThickness() {return thickness;}

    protected:
        std::vector<QPoint> points;
        Color color = {0, 0, 0};

        int thickness = 1;
};

#endif // SHAPE_H
