#include "line.h"
#include "color.h"
#include <QDebug>
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

//symmetric mid-point
//std::vector<Pixel> Line::getPixels()
//{
//     std::vector<Pixel> pixels;

//     int x1 = points[0].x();
//     int y1 = points[0].y();
//     int x2 = points[1].x();
//     int y2 = points[1].y();

//     const bool steep = abs(y2 - y1) > abs(x2 - x1);
//     if (steep) {
//         std::swap(x1,y1);
//         std::swap(x2,y2);
//     }
//     if (x1 > x2) {
//         std::swap(x1,x2);
//         std::swap(y1,y2);
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

//mid-point
std::vector<Pixel> Line::getPixels()
{
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

std::vector<Pixel> Line::getPixelsAA(){

    std::vector<Pixel> pixels;

    int x0 = points[0].x();
    int y0 = points[0].y();
    int x1 = points[1].x();
    int y1 = points[1].y();

    auto ipart = [](float x) -> int {return int(std::floor(x));};
    auto round = [](float x) -> float {return std::round(x);};
    auto fpart = [](float x) -> float {return x - std::floor(x);};
    auto rfpart = [=](float x) -> float {return 1 - fpart(x);};

    const bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0,y0);
        std::swap(x1,y1);
    }
    if (x0 > x1) {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float gradient = (dx == 0) ? 1 : dy/dx;

    int xpx11;
    float intery;
    {
        const float xend = round(x0);
        const float yend = y0 + gradient * (xend - x0);
        const float xgap = rfpart(x0 + 0.5);
        xpx11 = int(xend);
        const int ypx11 = ipart(yend);
        if (steep) {
            pixels.push_back(Pixel(ypx11, xpx11, rfpart(yend) * xgap));
            pixels.push_back(Pixel(ypx11 + 1, xpx11, fpart(yend) * xgap));
        } else {
            pixels.push_back(Pixel(xpx11, ypx11, rfpart(yend) * xgap));
            pixels.push_back(Pixel(xpx11, ypx11 + 1, fpart(yend) * xgap));
        }
        intery = yend + gradient;
    }

    int xpx12;
    {
        const float xend = round(x1);
        const float yend = y1 + gradient * (xend - x1);
        const float xgap = rfpart(x1 + 0.5);
        xpx12 = int(xend);
        const int ypx12 = ipart(yend);
        if (steep) {
            pixels.push_back(Pixel(ypx12, xpx12, rfpart(yend) * xgap));
            pixels.push_back(Pixel(ypx12 + 1, xpx12, fpart(yend) * xgap));
        } else {
            pixels.push_back(Pixel(xpx12, ypx12, rfpart(yend) * xgap));
            pixels.push_back(Pixel(xpx12, ypx12 + 1, fpart(yend) * xgap));
        }
    }

    if (steep) {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            pixels.push_back(Pixel(ipart(intery), x, rfpart(intery)));
            pixels.push_back(Pixel(ipart(intery) + 1, x, fpart(intery)));
            intery += gradient;
        }
    } else {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            pixels.push_back(Pixel(x, ipart(intery), rfpart(intery)));
            pixels.push_back(Pixel(x, ipart(intery) + 1, fpart(intery)));
            intery += gradient;
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

