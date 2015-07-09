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

    void stopStitch();

signals:

    void finish();

public slots:

private:
    std::vector<cv::Point> originPoint;

    std::vector<std::string> videoName;

    std::vector<cv::Mat> frame;

    bool stopped;

    void run();

    cv::Mat bgr2gray(cv::Mat src);
};

#endif // TRAJECTORY_TRACKING_H
