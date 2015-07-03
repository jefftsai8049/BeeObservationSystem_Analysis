#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv.hpp>
//#include <stitching.hpp>

#include <omp.h>

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QTime>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QVector>

#include "cam_input.h"

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

    void on_stitchingStart_pushButton_clicked();

    void on_stitchingStop_pushButton_clicked();

    void receivePano(cv::Mat pano);

    void stitchImage();

    void changeStitchMode();

private:
    Ui::MainWindow *ui;

    cam_input *stitcher;

    std::vector<std::string> getVideoName(QVector<QStringList> list, std::string path);

    int stitchMode = 0;//Manual

    std::vector<std::string> videoNames;

    QVector<QStringList> videoList;

    QDir dir;

};

#endif // MAINWINDOW_H
