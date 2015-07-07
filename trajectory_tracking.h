#ifndef TRAJECTORY_TRACKING_H
#define TRAJECTORY_TRACKING_H

#include <QThread>
#include <QObject>
#include <QDebug>

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

    void imageShift(std::vector<cv::Mat> stitchFrame,std::vector<cv::Point> originPoint);

    void imageShift(std::vector<cv::Mat> stitchFrame);

signals:

public slots:

private:
    std::vector<cv::Point> originPoint;

    void run();
};

#endif // TRAJECTORY_TRACKING_H
