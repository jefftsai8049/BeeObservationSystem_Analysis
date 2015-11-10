#ifndef OBJECT_TRACKING_H
#define OBJECT_TRACKING_H

#include <QObject>

#include <QDateTime>
#include <opencv.hpp>
#include <stdlib.h>

#include "math_function.h"

struct track
{
    std::vector<std::vector<char>> name;
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
    explicit object_tracking(QObject *parent = 0);
    ~object_tracking();

    void compute(const QDateTime &time, const std::vector<cv::Vec3f>& circles, const std::vector<std::string>& w1, const std::vector<std::string>& w2);

    void setImageRange(const cv::Size &imageRange);

    void lastPath(std::vector< std::vector<cv::Point> >& path);

    void drawPath(cv::Mat& src,const std::vector< std::vector<cv::Point> >& path);

signals:

public slots:

private:

    cv::Size range;

    std::vector<track> path;

};
#endif // OBJECT_TRACKING_H
