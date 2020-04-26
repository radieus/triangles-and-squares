#include "mainwindow.h"
#include "ui_mainwindow.h"

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


void MainWindow::on_drawLineButton_toggled(bool checked)
{
    if (checked){
        scene->myshape = DrawingArea::LINE;
    }
}

void MainWindow::on_drawCircleButton_toggled(bool checked)
{
    if (checked){
        scene->myshape = DrawingArea::CIRCLE;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QColor dialog_color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (dialog_color.isValid()){
        int _r, _g, _b;
        dialog_color.getRgb(&_r, &_g, &_b);
        //active->color = Color(_r, _g, _b);
    }
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if (checked){
        scene->mymode = DrawingArea::TRANSFORM;
    }
}
