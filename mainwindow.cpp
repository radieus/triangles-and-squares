#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawingarea.h"
#include <iostream>
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

void MainWindow::on_drawRectangleButton_toggled(bool checked)
{
    if (checked) {
        scene->myshape = DrawingArea::RECTANGLE;
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
    if (fileName.isEmpty())
        return;

    json j;
    for (auto &shape : scene->shapes) {
        j.push_back(shape->getJsonFormat());
    }

    std::ofstream file(fileName.toStdString());
        file << j;
}

void MainWindow::on_actionLoad_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.json)"));
    if (fileName.isEmpty())
        return;

    std::ifstream i(fileName.toStdString());
    json j;
    i >> j;

    for (auto &shape : j) {
        if (shape["shape"].get<std::string>() == "circle") {
            std::unique_ptr<Shape> current = std::make_unique<Circle>(QPoint(shape["points"][0][0], shape["points"][0][1]),
                                                                      QPoint(shape["points"][1][0], shape["points"][1][1]));
            current->setColor(Color(shape["color"][0], shape["color"][1], shape["color"][2]));
            current->setThickness(shape["thickness"].get<int>());
            scene->shapes.push_back(std::move(current));
            update();
        }

        else if (shape["shape"].get<std::string>() == "line") {
            std::unique_ptr<Shape> current = std::make_unique<Line>(QPoint(shape["points"][0][0], shape["points"][0][1]),
                                                                    QPoint(shape["points"][1][0], shape["points"][1][1]));
            current->setColor(Color(shape["color"][0], shape["color"][1], shape["color"][2]));
            current->setThickness(shape["thickness"].get<int>());
            scene->shapes.push_back(std::move(current));
            update();
        }

        else if (shape["shape"].get<std::string>() == "polygon") {
            std::unique_ptr<Shape> current = std::make_unique<Polygon>();
            for (auto point : shape["points"]) {
                current->addPoint(QPoint(point[0], point[1]));
            }

            current->setColor(Color(shape["color"][0], shape["color"][1], shape["color"][2]));
            current->setThickness(shape["thickness"].get<int>());
            scene->shapes.push_back(std::move(current));
            update();
        }

        else if (shape["shape"].get<std::string>() == "rectangle") {
            std::unique_ptr<Shape> current = std::make_unique<Rectangle>(QPoint(shape["points"][0][0], shape["points"][0][1]),
                                                                         QPoint(shape["points"][1][0], shape["points"][1][1]));

            current->setColor(Color(shape["color"][0], shape["color"][1], shape["color"][2]));
            current->setThickness(shape["thickness"].get<int>());
            scene->shapes.push_back(std::move(current));
            update();
        }
    }
}


void MainWindow::on_removeButton_clicked()
{
    for (auto &shape : scene->shapes){
        if (scene->activeShape == &shape){
            scene->shapes.erase(std::remove(scene->shapes.begin(), scene->shapes.end(), shape), scene->shapes.end());
            update();
            break;
        }
    }
}

void MainWindow::on_antialiasBox_clicked()
{
    if (scene->antialiased == false) {
        scene->antialiased = true;
        update();
    }
    else {
        scene->antialiased = false;
        update();
    }
}

