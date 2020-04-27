#include "drawingarea.h"
#include "line.h"
#include "circle.h"
#include "polygon.h"
#include "arc.h"

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent)
{
    image = QImage(width(), height(), QImage::Format_BGR888);
}

void DrawingArea::_resize()
{
    image = QImage(width(), height(), QImage::Format_BGR888);
}

bool operator==(const Pixel& lhs, const Pixel& rhs)
{
    return lhs.x== rhs.x && lhs.y == rhs.y;
}

bool DrawingArea::setPixel(int x, int y, Color color)
{
    uchar* ptr = image.bits();
    if (x > image.width() || x < 0 || y >= image.height() || y < 0){
        return false;
    }

    *(ptr + 3*x + 3*y*image.height()) = color.r;
    *(ptr + 3*x + 3*y*image.height() + 1) = color.g;
    *(ptr + 3*x + 3*y*image.height() + 2) = color.b;

    return true;
}

void DrawingArea::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    uchar* ptr = image.bits();
    uchar* end_ptr = ptr + image.sizeInBytes();

    while (ptr < end_ptr) {
        *ptr = 255;
        ptr++;
    }

    for (const auto &shape : shapes) {
          std::vector<Pixel> pixels = shape->getPixels();
          for (Pixel pix: pixels){
              setPixel(pix.x, pix.y, shape->getColor());
          }
      }

    painter.drawImage(0, 0, image);
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        switch (mymode) {
        case DRAW:
            qDebug() << startPoint.x() << " "<< startPoint.y();
            startPoint = event->pos();
            break;
        case TRANSFORM:
            startPoint = event->pos();
            break;
        }
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    int pos_x = event->pos().x();
    int pos_y = event->pos().y();

}

void DrawingArea::changeColorOfActiveShape(Color color)
{
    (*activeShape)->setColor(color);
    update();
}

void DrawingArea::setShapeThickness(int thickness)
{
    (*activeShape)->setThickness(thickness);
    update();
}

void DrawingArea::eraseShapes()
{
    shapes.clear();
    update();
}


void DrawingArea::paintPolygon()
{
    shapes.push_back(std::move(polygon));
    finished = true;
    newPolygon = true;
    polygon = nullptr;
    update();
}

void DrawingArea::paintArc()
{
    shapes.push_back(std::move(arc));
    finished = true;
    newArc = true;
    arc = nullptr;
    update();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        switch (mymode) {
        case DRAW:
        {
            endPoint = event->pos();
            break;
        }
        case TRANSFORM:
        {
            myshape = SELECT;
            endPoint = event->pos();
            break;
        }
        }

        switch (myshape) {
        case LINE:
        {
            qDebug() << startPoint.x() << " "<< startPoint.y() << " "<< endPoint.x() << " "<< endPoint.y();
            std::unique_ptr<Shape> line = std::make_unique<Line>(startPoint, endPoint);
            shapes.push_back(std::move(line));
            update();
            break;
        }
        case CIRCLE:
        {
            qDebug() << startPoint.x() << " "<< startPoint.y() << " "<< endPoint.x() << " "<< endPoint.y();
            std::unique_ptr<Shape> circle = std::make_unique<Circle>(startPoint, endPoint);
            shapes.push_back(std::move(circle));
            update();
            break;
        }
        case ARC:
        {
            if (newArc) {
                arc = std::make_unique<Arc>();
                arc->addPoint(startPoint);
                newArc = false;
            }
            else {
                arc->addPoint(startPoint);
            }

            break;
        }

        case POLYGON:
        {
            if (newPolygon) {
                polygon = std::make_unique<Polygon>();
                polygon->addPoint(startPoint);
                newPolygon = false;
            }
            else {
                polygon->addPoint(startPoint);
            }

            break;
        }
        case SELECT:
        {
            Pixel tmp_pix(startPoint.x(), startPoint.y());
            for (auto &shape : shapes) {
                  std::vector<Pixel> pixels = shape->getPixels();
                  for (Pixel pix: pixels){
                      if (tmp_pix == pix){
                         qDebug() << "Pixel found:" << pix.x <<" "<< pix.y;
                         activeShape = &shape;
                         break;
                      }
                  }
            }

            break;

        }
        }
    }
}

