#include "circle.h"
#include <cmath>
#include <QDebug>

Circle::Circle()
{
}

Circle::Circle(QPoint p1, QPoint p2)
{
    points.push_back(p1);
    points.push_back(p2);
    radius = sqrt((p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y()));
}

void Circle::addPoint(QPoint p)
{
    points.push_back(p);
}

QPoint Circle::getPoint(int i)
{
    return points[i];
}

std::vector<Pixel> Circle::getPixelsAA()
{
    std::vector<Pixel> pixels;

    int x1 = points[0].x();
    int y1 = points[0].y();

    for(int i = -thickness/2; i < thickness/2; i++) {

        int radius_ = radius + i;
        int dE = 3;
        int dSE = 5 - 2 * radius_;
        int d = 1 - radius_;
        int x = radius_;
        int y = 0;

        pixels.push_back(Pixel(x1, y1 + radius_));
        pixels.push_back(Pixel(x1, y1 - radius_));
        pixels.push_back(Pixel(x1 + radius_, y1));
        pixels.push_back(Pixel(x1 - radius_, y1));

        while (x > y)
        {
            y++;
            if (d <= 0) {
                d += dE;
                dE += 2;
                dSE += 2;
            }
            else {
                d += dSE;
                dE += 2;
                dSE += 4;
                x--;
            }

            if (x < y) break;

            pixels.push_back(Pixel(x1+x,y1+y));
            pixels.push_back(Pixel(x1-x,y1+y));
            pixels.push_back(Pixel(x1+x,y1-y));
            pixels.push_back(Pixel(x1-x,y1-y));
            pixels.push_back(Pixel(x1+y,y1+x));
            pixels.push_back(Pixel(x1+y,y1-x));
            pixels.push_back(Pixel(x1-y,y1+x));
            pixels.push_back(Pixel(x1-y,y1-x));

        }
    }

    return pixels;
}


std::vector<Pixel> Circle::getPixels()
{
    std::vector<Pixel> pixels;

    int x1 = points[0].x();
    int y1 = points[0].y();

    float radiusX = radius;
    float radiusY = radius;
    float radiusX2 = radiusX * radiusX;
    float radiusY2 = radiusY * radiusY;

    float maxTransparency = 255;

    pixels.push_back(Pixel(x1, y1 + radius));
    pixels.push_back(Pixel(x1, y1 - radius));
    pixels.push_back(Pixel(x1 + radius, y1));
    pixels.push_back(Pixel(x1 - radius, y1));

    float quarter = roundf(radiusX2 / sqrtf(radiusX2 + radiusY2));
    for(float _x = 0; _x <= quarter; _x++) {

        float _y = radiusY * sqrtf(1 - _x * _x / radiusX2);
        float error = _y - floorf(_y);

        float transparency = roundf(error * maxTransparency);
        double brightness1 = transparency/255;
        double brightness2 = maxTransparency/255 - transparency/255;

        //up down
        pixels.push_back(Pixel(x1 + _x, y1 + floorf(_y), brightness1));
        pixels.push_back(Pixel(x1 - _x, y1 + floorf(_y), brightness1));
        pixels.push_back(Pixel(x1 + _x, y1 - floorf(_y) + 1, brightness2));
        pixels.push_back(Pixel(x1 - _x, y1 - floorf(_y) + 1, brightness2));
        pixels.push_back(Pixel(x1 + _x, y1 + floorf(_y) - 1, brightness2));
        pixels.push_back(Pixel(x1 - _x, y1 + floorf(_y) - 1, brightness2));
        pixels.push_back(Pixel(x1 + _x, y1 - floorf(_y), brightness1));
        pixels.push_back(Pixel(x1 - _x, y1 - floorf(_y), brightness1));
    }

    quarter = roundf(radiusY2 / sqrtf(radiusX2 + radiusY2));
    for(float _y = 0; _y <= quarter; _y++) {

        float _x = radiusX * sqrtf(1 - _y * _y / radiusY2);
        float error = _x - floorf(_x);

        float transparency = roundf(error * maxTransparency);
        double brightness1 = transparency/255;
        double brightness2 = (maxTransparency - transparency)/255;

        //left right
        pixels.push_back(Pixel(x1 + floorf(_x), y1 + floorf(_y), brightness1));
        pixels.push_back(Pixel(x1 - floorf(_x) + 1, y1 + floorf(_y), brightness2));
        pixels.push_back(Pixel(x1 + floorf(_x), y1 - floorf(_y), brightness1));
        pixels.push_back(Pixel(x1 - floorf(_x) + 1, y1 - floorf(_y), brightness2));
        pixels.push_back(Pixel(x1 + floorf(_x) - 1, y1 + floorf(_y), brightness2));
        pixels.push_back(Pixel(x1 - floorf(_x) , y1 + floorf(_y), brightness1));
        pixels.push_back(Pixel(x1 + floorf(_x) - 1, y1 - floorf(_y), brightness2));
        pixels.push_back(Pixel(x1 - floorf(_x), y1 - floorf(_y), brightness1));
    }
    return pixels;
}

json Circle::getJsonFormat()
{
    json item;
    item["shape"] = "circle";
    for (auto point : getPoints()) {
        item["points"].push_back({point.x(), point.y()});
    }
    item["color"] = {getColor().r, getColor().g, getColor().b};
    item["thickness"] = getThickness();
    item["radius"] = radius;

    return item;
}

