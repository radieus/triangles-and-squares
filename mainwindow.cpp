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


