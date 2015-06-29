#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv.hpp>

#include <omp.h>

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QTime>
#include <QList>
#include <QString>
#include <QStringList>

#define fps 12

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLoad_Raw_Video_File_triggered();


    void on_actionLoad_Stitching_Image_triggered();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
