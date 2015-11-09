#ifndef OBJECT_TRACKING_H
#define OBJECT_TRACKING_H

#include <QObject>

#include <QDateTime>
#include <opencv.hpp>
#include <stdlib.h>

struct track
{
    std::vector<char[2]> name;
    std::vector<cv::Point> position;

    std::vector<QDateTime> time;

    int size();
    cv::Point lastPosition();

};

class object_tracking : public QObject
{
    Q_OBJECT
public:
    explicit object_tracking(QObject *parent = 0);
    ~object_tracking();

signals:

public slots:

private:

};
#endif // OBJECT_TRACKING_H
