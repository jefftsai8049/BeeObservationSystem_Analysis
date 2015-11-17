#ifndef OBJECT_TRACKING_H
#define OBJECT_TRACKING_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDataStream>
#include <opencv.hpp>
#include <stdlib.h>

#include "math_function.h"

#define REMAIN_SIZE 20
#define FORGET_TRACKING_TIME 5

struct track
{
    std::vector<char> w1;
    std::vector<char> w2;
    std::vector<cv::Point> position;

    std::vector<QDateTime> time;

    int size();
    cv::Point lastPosition();
    cv::Point futurePosition();

    double howClose(const cv::Vec3f& circle);

};

class object_tracking : public QObject
{
    Q_OBJECT
public:
    explicit object_tracking(QObject *parent = 0,const QDateTime fileTime = QDateTime());
    ~object_tracking();

    void compute(const QDateTime &time, const std::vector<cv::Vec3f>& circles, const std::vector<std::string>& w1, const std::vector<std::string>& w2);

    void setImageRange(const cv::Size &imageRange);

    void lastPath(std::vector< std::vector<cv::Point> >& path);

    void drawPath(cv::Mat &src);

    void savePath();

    void saveAllPath();

signals:

public slots:



private:

    QDateTime nowTime;

    QDateTime startTime;

    cv::Size range;

    std::vector<track> path;



};
#endif // OBJECT_TRACKING_H
