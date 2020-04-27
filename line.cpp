#include "line.h"
#include "color.h"

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

std::vector<Pixel> Line::getPixels()
{
     std::vector<Pixel> pixels;

     int x1 = points[0].x();
     int y1 = points[0].y();
     int x2 = points[1].x();
     int y2 = points[1].y();

     if (abs(y2 - y1) > abs(x2 - x1)) {
         std::swap(x1, y1);
         std::swap(x2, y2);
     }

     if (x1 > x2) {
         std::swap(x1, x2);
         std::swap(y1, y2);
     }

     int dx = x2 - x1;
     int dy = abs(y2 - y1);
     int d = 2*dy - dx;
     int dE = 2*dy;
     int dNE = 2*(dy - dx);
     int xf = x1, yf = y1;
     int xb = x2, yb = y2;

     int ystep = (y1 < y2) ? 1 : -1;

     pixels.push_back(Pixel(xf, yf));
     pixels.push_back(Pixel(xb, yb));

     while (xf < xb) {
         ++xf;
         --xb;
         if (d < 0)
             d += dE;
         else {
             d += dNE;
             yf += ystep;
             yb -= ystep;
         }

         pixels.push_back(Pixel(xf, yf));
         pixels.push_back(Pixel(xb, yb));

     }

     return pixels;
}
