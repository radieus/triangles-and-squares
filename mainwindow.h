#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSizePolicy>
#include <drawingarea.h>
#include <QColorDialog>
#include <QColor>
#include <QFileDialog>
#include <Shape.h>
#include <nlohmann/json.hpp>

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

    void on_pushButton_clicked();

    void on_radioButton_toggled(bool checked);

    void on_clearButton_clicked();

    void on_drawPolygonButton_toggled(bool checked);

    void on_paintPolygon_clicked();

    void on_thiccSlasher_sliderReleased();

    void on_drawArcButton_toggled(bool checked);

    void on_paintArc_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

    void on_antialiasBox_toggled(bool checked);

    void on_removeButton_clicked();

private:
    Ui::MainWindow *ui;
    DrawingArea *scene;
};
#endif // MAINWINDOW_H
