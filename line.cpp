#include "line.h"
#include "color.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

Line::Line()
{
}

Line::Line(QPoint p1, QPoint p2)
{
    points.push_back(p1);
    points.push_back(p2);

}

void Line::addPoint(QPoint p)
{
    points.push_back(p);
}

QPoint Line::getPoint(int i)
{
    return points[i];
}

//std::vector<Pixel> Line::getPixels()
//{
//     std::vector<Pixel> pixels;

//     int x1 = points[0].x();
//     int y1 = points[0].y();
//     int x2 = points[1].x();
//     int y2 = points[1].y();

//     if (abs(y2 - y1) > abs(x2 - x1)) {
//         std::swap(x1, y1);
//         std::swap(x2, y2);
//     }

//     if (x1 > x2) {
//         std::swap(x1, x2);
//         std::swap(y1, y2);
//     }

//     int dx = x2 - x1;
//     int dy = abs(y2 - y1);
//     int d = 2*dy - dx;
//     int dE = 2*dy;
//     int dNE = 2*(dy - dx);
//     int xf = x1, yf = y1;
//     int xb = x2, yb = y2;

//     int ystep = (y1 < y2) ? 1 : -1;

//     pixels.push_back(Pixel(xf, yf));
//     pixels.push_back(Pixel(xb, yb));

//     while (xf < xb) {
//         ++xf;
//         --xb;
//         if (d < 0)
//             d += dE;
//         else {
//             d += dNE;
//             yf += ystep;
//             yb -= ystep;
//         }

//         pixels.push_back(Pixel(xf, yf));
//         pixels.push_back(Pixel(xb, yb));

//     }

//     return pixels;
//}

std::vector<Pixel> Line::getPixels()
{   //https://community.khronos.org/t/drawing-line-bresenhem-midpoint-algorithm/58759/7
    std::vector<Pixel> pixels;

    int x1 = points[0].x();
    int y1 = points[0].y();
    int x2 = points[1].x();
    int y2 = points[1].y();

    int	y, x, dy, dx, stepx, stepy;
    int	d, incE, incNE;

    dx = x2 - x1;
    dy = y2 - y1;

    stepx = sgn(dx);
    stepy = sgn(dy);

    dx = abs(dx);
    dy = abs(dy);

    if (dy > dx) {
        incE = 2 * dx;
        incNE = 2 * dx - 2 * dy;
        d = 2 * dy - dx;

        x = x1;
        y = y1;

        while (y != y2) {
            for(int i = -thickness/2; i < thickness/2; i++){
                pixels.push_back(Pixel(x + i, y));
            }

            if (d <= 0)
                d += incE;
            else {
                d += incNE;
                x += stepx;
            }
            y += stepy;
        }
    } else {
        incE = 2 * dy;
        incNE = 2 * dy - 2 * dx;
        d = 2 * dy - dx;

        x = x1;
        y = y1;
        while (x != x2) {
            for(int i = -thickness/2; i < thickness/2; i++){
                pixels.push_back(Pixel(x, y + i));
            }
            if (d <= 0)
                d += incE;
            else {
                d += incNE;
                y += stepy;
            }
            x += stepx;
        }
    }

    return pixels;
}

json Line::getJsonFormat()
{
    json item;
    item["shape"] = "line";
    for (auto point : getPoints()) {
        item["points"].push_back({point.x(), point.y()});
    }
    item["color"] = {getColor().r, getColor().g, getColor().b};
    item["thickness"] = getThickness();

    return item;
}

