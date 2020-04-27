#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawingarea.h"
#include <fstream>

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new DrawingArea(ui->blackboard);
    scene->resize(ui->blackboard->width(), ui->blackboard->height());
    scene->_resize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

void MainWindow::on_drawLineButton_toggled(bool checked)
{
    if (checked) {
        scene->myshape = DrawingArea::LINE;
        scene->mymode = DrawingArea::DRAW;
    }
}

void MainWindow::on_drawCircleButton_toggled(bool checked)
{
    if (checked) {
        scene->myshape = DrawingArea::CIRCLE;
        scene->mymode = DrawingArea::DRAW;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QColor dialog_color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (dialog_color.isValid()) {
        int _r, _g, _b;
        dialog_color.getRgb(&_r, &_g, &_b);
        scene->changeColorOfActiveShape(Color(_b, _g, _r));
    }
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if (checked) {
        scene->mymode = DrawingArea::TRANSFORM;
        qDebug()<<scene->mymode;
    }
}

void MainWindow::on_clearButton_clicked()
{
    scene->eraseShapes();
}

void MainWindow::on_drawPolygonButton_toggled(bool checked)
{
    if (checked) {
        scene->myshape = DrawingArea::POLYGON;
        scene->mymode = DrawingArea::DRAW;
    }
}

void MainWindow::on_paintPolygon_clicked()
{
    if (scene->myshape == DrawingArea::POLYGON) {
        scene->paintPolygon();
    }
}

void MainWindow::on_thiccSlasher_sliderReleased()
{
    scene->setShapeThickness(ui->thiccSlasher->value());
}

void MainWindow::on_drawArcButton_toggled(bool checked)
{
    if (checked) {
        scene->myshape = DrawingArea::ARC;
        scene->mymode = DrawingArea::DRAW;
    }
}

void MainWindow::on_paintArc_clicked()
{
    if (scene->myshape == DrawingArea::ARC) {
        scene->paintArc();
    }
}

void MainWindow::on_actionSave_triggered()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save Your Project"), "", tr("Project File(*.json);;All Files (*)"));
    json j;
    for (auto &shape : scene->shapes) {
        j.push_back(shape->getJsonFormat());
    }

    std::ofstream file(fileName.toStdString());
        file << j;
}

void MainWindow::on_actionLoad_triggered()
{

}
