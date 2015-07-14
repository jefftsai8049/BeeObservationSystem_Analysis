#ifndef TRAJECTORY_TRACKING_H
#define TRAJECTORY_TRACKING_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTime>

#include <stdlib.h>
#include <math.h>


#include "opencv.hpp"


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

    cv::Mat imageShift(std::vector<cv::Mat> stitchFrame);

    void setVideoName(std::vector<std::string> videoName);

    void setHoughCircleParameters(const int &dp,const int &miniDist,const int &para_1,const int &para_2,const int &minRadius,const int &maxRadius);

    void setShowImage(const bool &status);

    void stopStitch();

signals:

    void finish();

    void sendFPS(const double &fps);

public slots:

private:
    std::vector<cv::Point> originPoint;

    std::vector<std::string> videoName;

    std::vector<cv::Mat> frame;

    bool stopped;

    bool showImage = true;

    void run();

    cv::Mat bgr2gray(cv::Mat src);

    //Hough Circle Parameters
    int dp = 2;

    int miniDist = 20;

    int para_1 = 80;

    int para_2 = 80;

    int minRadius = 12;

    int maxRadius = 19;
};

#endif // TRAJECTORY_TRACKING_H
