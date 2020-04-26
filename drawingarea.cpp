#include "drawingarea.h"
#include "line.h"

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent)
{
    image = QImage(width(), height(), QImage::Format_BGR888);
}

void DrawingArea::_resize()
{
    image = QImage(width(), height(), QImage::Format_BGR888);
}

bool DrawingArea::setPixel(int x, int y, Color color)
{
    uchar* ptr = image.bits();
    if (x > image.width() || y > image.height()){
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
              setPixel(pix.x, pix.y, Color(0, 0, 0));
          }
      }

    painter.drawImage(0, 0, image);
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        switch (mode) {
        case DRAW:
            drawing = true;
            startPoint = event->pos();
            break;
        case ERASE:
            drawing = true;
            startPoint = event->pos();
            break;
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        switch (mode) {
        case DRAW:
            drawing = false;
            endPoint = event->pos();
            break;
        case ERASE:
            drawing = false;
            endPoint = event->pos();
            break;
        }

        qDebug() << startPoint.x() << " "<< startPoint.y() << " "<< endPoint.x() << " "<< endPoint.y();

        std::unique_ptr<Shape> line = std::make_unique<Line>(startPoint, endPoint);

        shapes.push_back(std::move(line));

        update();

    }
}

