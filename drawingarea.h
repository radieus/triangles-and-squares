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
#include "arc.h"
#include "circle.h"
#include "rectangle.h"

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);
    void clearImage();
    bool setPixel(int x, int y, Color color, double brightness);
    enum myMode { DRAW, TRANSFORM };
    enum myShape { LINE, CIRCLE, POLYGON, ARC, RECTANGLE, SELECT};
    void _resize();

    myMode mymode = DRAW;
    myShape myshape = LINE;
    std::unique_ptr<Shape>* activeShape = nullptr;
    void changeColorOfActiveShape(Color color);
    void eraseShapes();
    void paintPolygon();
    void paintArc();
    void setShapeThickness(int thickness);
    std::vector<std::unique_ptr<Shape> > shapes;
    bool finished = false;
    bool antialiased = false;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent * event) override;

private:
    QImage image;
    QPoint startPoint;
    QPoint endPoint;
    std::vector<std::unique_ptr<Shape> > trash;
    std::unique_ptr<Polygon> polygon = nullptr;
    std::unique_ptr<Arc> arc = nullptr;

    bool newPolygon = true;
    bool newArc = true;

};

#endif // DRAWINGAREA_H
