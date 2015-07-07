#ifndef TRAJECTORY_TRACKING_H
#define TRAJECTORY_TRACKING_H

#include <QObject>
#include <QDebug>

#include <stdlib.h>
#include <math.h>


#include "opencv.hpp"


#define imgSizeX 1200
#define imgSizeY 1600

class trajectory_tracking : public QObject
{
    Q_OBJECT
public:
    explicit trajectory_tracking(QObject *parent = 0);
    ~trajectory_tracking();

    void imageShift(std::vector<cv::Mat> stitchFrame,std::vector<cv::Point> originPoint);

signals:

public slots:

private:

};

#endif // TRAJECTORY_TRACKING_H
