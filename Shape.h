#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <vector>
#include "color.h"
#include "pixel.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Shape {
    public:
        virtual void addPoint(QPoint p) = 0;
        virtual QPoint getPoint(int i) = 0;

        virtual std::vector<Pixel> getPixels() = 0;

        virtual json getJsonFormat() = 0;

        Color getColor() {return color;}
        void setColor(Color color) {this->color = color;}

        std::vector<QPoint> getPoints() {return points;}

        void setThickness(int t) {thickness = t;}
        int getThickness() {return thickness;}

        virtual ~Shape() {};

    protected:
        std::vector<QPoint> points;
        Color color = {0, 0, 0};

        int thickness = 2;
};

#endif // SHAPE_H
