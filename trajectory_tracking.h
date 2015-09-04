#ifndef TRAJECTORY_TRACKING_H
#define TRAJECTORY_TRACKING_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTime>

#include <stdlib.h>
#include <math.h>

//#include "cudaimgproc.hpp"
#include <opencv.hpp>
#include <core/ocl.hpp>

#include "tag_recognition.h"




#define imgSizeX 1200
#define imgSizeY 1600

class trajectory_tracking : public QThread
{
    Q_OBJECT
public:
    explicit trajectory_tracking(QObject *parent = 0);
    ~trajectory_tracking();

    void setImageShiftOriginPoint(std::vector<cv::Point> originPoint);

    cv::Mat imageShift(std::vector<cv::Mat> stitchFrame,std::vector<cv::Point> originPoint);

    cv::Mat imageShiftLoaded(std::vector<cv::Mat> stitchFrame);

    void setVideoName(std::vector<std::string> videoName);

    void setHoughCircleParameters(const int &dp,const int &minDist,const int &para_1,const int &para_2,const int &minRadius,const int &maxRadius);

    void setShowImage(const bool &status);

    void stopStitch();

signals:

    void finish();

    void sendFPS(const double &fps);

public slots:

private:
    tag_recognition *TR;

    std::vector<cv::Point> originPoint;

    std::vector<std::string> videoName;

    std::vector<cv::Mat> frame;


    bool stopped;

    bool showImage = true;

    //Hough Circle Parameters
    int dp = 2;

    int minDist = 20;

    int para_1 = 150;

    int para_2 = 25;

    int minRadius = 6;

    int maxRadius = 9;

    void run();

    cv::Mat imageCutBlack(cv::Mat src);

};

#endif // TRAJECTORY_TRACKING_H
