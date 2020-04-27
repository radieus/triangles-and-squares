#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <memory>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QTime>
#include <QDebug>
#include "Shape.h"
#include "color.h"
#include "polygon.h"

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);
    void clearImage();
    bool setPixel(int x, int y, Color color);
    enum myMode { DRAW, TRANSFORM };
    enum myShape { LINE, CIRCLE, POLYGON, SELECT};
    void _resize();

    myMode mymode = DRAW;
    myShape myshape = LINE;
    std::unique_ptr<Shape>* activeShape;
    void changeColorOfActiveShape(Color color);
    void eraseShapes();
    void paintPolygon();

    bool finished = false;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QImage image;
    QPoint startPoint;
    QPoint endPoint;
    std::vector<std::unique_ptr<Shape> > shapes;
    std::unique_ptr<Polygon> polygon = nullptr;

    bool newPolygon = true;

};

#endif // DRAWINGAREA_H
