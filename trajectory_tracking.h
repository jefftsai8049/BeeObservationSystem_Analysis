#ifndef TRAJECTORY_TRACKING_H
#define TRAJECTORY_TRACKING_H

#include <QObject>
#include <QDebug>

#include <stdlib.h>


#include "opencv.hpp"


class trajectory_tracking : public QObject
{
    Q_OBJECT
public:
    explicit trajectory_tracking(QObject *parent = 0);
    ~trajectory_tracking();

    void imageShift(std::vector<cv::Mat> stitchFrame,std::vector<int> shiftDelta);

signals:

public slots:
};

#endif // TRAJECTORY_TRACKING_H
