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
#include "trajectory_tracking.h"

#define fps 12
#define imgSizeX 1200
#define imgSizeY 1600
//int mouseLeft = 0;



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
    void receivePano(cv::Mat pano);

    void receiveFPS(const double &fpsRun);

    void stitchImage();

    void changeStitchMode();

    void on_actionLoad_Raw_Video_File_triggered();

    void on_actionLoad_Stitching_Image_triggered();

    void on_stitchingStart_pushButton_clicked();

    void on_stitchingStop_pushButton_clicked();

    void on_actionLoad_Maunal_Stitching_Setting_triggered();

    void on_stitching_pushButton_clicked();

    void on_dp_hough_circle_spinBox_valueChanged(int arg1);

    void on_minDist_hough_circle_spinBox_valueChanged(int arg1);

    void on_para_1_hough_circle_spinBox_valueChanged(int arg1);

    void on_para_2_hough_circle_spinBox_valueChanged(int arg1);

    void on_minRadius_hough_circle_spinBox_valueChanged(int arg1);

    void on_maxRadius_hough_circle_spinBox_valueChanged(int arg1);

    void on_show_image_checkBox_clicked();

    void on_cuda_test_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    cam_input *stitcher;

    trajectory_tracking *TT;

//    trajectory_tracking *TT;

    std::vector<std::string> getVideoName(QVector<QStringList> list, std::string path);



    int stitchMode = 0;//Manual

    bool manualLoad = 0;

    std::vector<std::string> videoNames;

    QVector<QStringList> videoList;

    QDir dir;

};

#endif // MAINWINDOW_H
