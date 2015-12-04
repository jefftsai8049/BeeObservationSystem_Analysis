#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv.hpp>
#include <core/ocl.hpp>
#include <features2d.hpp>
#include "ml.hpp"

#include <omp.h>

#include <QMainWindow>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTime>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QVector>

#include "trajectory_tracking.h"
#include "tag_recognition.h"

#include "qsmartgraphicsview/qsmartgraphicsview.h"
#include "qsmartgraphicsview/qsmartlabel.h"


#define imgSizeX 1200
#define imgSizeY 1600




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

    void receiveShowImage(const cv::Mat &src);

    void receiveSystemLog(const QString &msg);

    void receiveProcessingProgress(const int &percentage);

    void stitchImage();

    void on_actionLoad_Raw_Video_File_triggered();

    void on_actionLoad_Stitching_Image_triggered();

    void on_stitchingStart_pushButton_clicked();

    void on_stitchingStop_pushButton_clicked();

    void on_stitching_pushButton_clicked();

    void on_dp_hough_circle_spinBox_valueChanged(int arg1);

    void on_minDist_hough_circle_spinBox_valueChanged(int arg1);

    void on_para_1_hough_circle_spinBox_valueChanged(int arg1);

    void on_para_2_hough_circle_spinBox_valueChanged(int arg1);

    void on_minRadius_hough_circle_spinBox_valueChanged(int arg1);

    void on_maxRadius_hough_circle_spinBox_valueChanged(int arg1);

    void on_show_image_checkBox_clicked();

    void on_load_training_data_pushButton_clicked();

    void on_test_recognition_pushButton_clicked();

    void on_binarythreshold_spinBox_valueChanged(int arg1);

    void on_actionChange_SVM_Model_triggered();

    void on_actionChange_PCA_Model_triggered();

    void on_actionChange_Stitching_Model_triggered();

    void on_actionTrain_New_Tag_Model_triggered();


    void on_actionLoad_Analysis_Data_triggered();

    void on_erase_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    trajectory_tracking *TT;

    tag_recognition *TR;

    cv::PCA *pca;

    std::vector<std::string> getVideoName(QVector<QStringList> list, std::string path);



    int stitchMode = 0;//Manual

    bool manualLoad = 0;

    std::vector<std::string> videoNames;

    QVector<QStringList> videoList;

    QDir dir;

    QFile systemLog;

signals:
    void sendSystemLog(const QString& msg);

};

#endif // MAINWINDOW_H
