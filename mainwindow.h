#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSizePolicy>
#include <drawingarea.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_drawLineButton_toggled(bool checked);

    void on_drawCircleButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    DrawingArea *scene;
};
#endif // MAINWINDOW_H
