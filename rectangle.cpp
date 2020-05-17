#include "rectangle.h"
#include <QDebug>

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(QPoint p1, QPoint p2)
{
    points.push_back(p1);
    points.push_back(p2);
    Line newLine = Line(QPoint(p1.x(), p1.y()), QPoint(p1.x(), p2.y()));
    lines.push_back(newLine);
    newLine = Line(QPoint(p1.x(), p1.y()), QPoint(p2.x(), p1.y()));
    lines.push_back(newLine);
    newLine = Line(QPoint(p1.x(), p2.y()), QPoint(p2.x(), p2.y()));
    lines.push_back(newLine);
    newLine = Line(QPoint(p2.x(), p1.y()), QPoint(p2.x(), p2.y()));
    lines.push_back(newLine);
}

void Rectangle::addPoint(QPoint p)
{
    points.push_back(p);
}

QPoint Rectangle::getPoint(int i)
{
    return points[i];
}

std::vector<Pixel> Rectangle::getPixels()
{
    std::vector<Pixel> pixels;

    for (auto line: lines) {
        line.setThickness(thickness);
        std::vector<Pixel> newPixels = line.getPixels();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    return pixels;
}

std::vector<Pixel> Rectangle::getPixelsAA()
{
    std::vector<Pixel> pixels;

    for (auto line: lines) {
        line.setThickness(thickness);
        std::vector<Pixel> newPixels = line.getPixelsAA();
        pixels.insert(pixels.end(), newPixels.begin(), newPixels.end());
    }

    return pixels;
}

json Rectangle::getJsonFormat()
{
    json item;
    item["shape"] = "Rectangle";
    for (auto point : getPoints()) {
        item["points"].push_back({point.x(), point.y()});
    }
    item["color"] = {getColor().r, getColor().g, getColor().b};
    item["thickness"] = getThickness();

    return item;
}

