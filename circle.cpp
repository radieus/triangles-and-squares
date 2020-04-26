#include "circle.h"
#include <cmath>

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

std::vector<Pixel> Circle::getPixels()
{
    std::vector<Pixel> pixels;

    int x1 = points[0].x();
    int y1 = points[0].y();

    int dE = 3;
    int dSE = 5 - 2 * radius;
    int d = 1 - radius;
    int x = radius;
    int y = 0;

    pixels.push_back(Pixel(x1, y1 + radius));
    pixels.push_back(Pixel(x1, y1 -radius));
    pixels.push_back(Pixel(x1 + radius, y1));
    pixels.push_back(Pixel(x1 -radius, y1));

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
        pixels.push_back(Pixel(x1+x,y1+y));
        pixels.push_back(Pixel(x1-x,y1+y));
        pixels.push_back(Pixel(x1+x,y1-y));
        pixels.push_back(Pixel(x1-x,y1-y));
        pixels.push_back(Pixel(x1+y,y1+x));
        pixels.push_back(Pixel(x1+y,y1-x));
        pixels.push_back(Pixel(x1-y,y1+x));
        pixels.push_back(Pixel(x1-y,y1-x));
    }

    return pixels;
}


