#ifndef TAG_RECOGNITION_H
#define TAG_RECOGNITION_H

#define tagSize 24
#define tagBinaryThreshold 100


#include <QObject>
#include <QDebug>

#include <opencv.hpp>

class tag_recognition : public QObject
{
    Q_OBJECT
public:
    explicit tag_recognition(QObject *parent = 0);
    ~tag_recognition();

    cv::Mat tagImgProc(cv::Mat src);

signals:

public slots:
private:
    cv::Mat calcualteCOV(cv::KeyPoint points);
};

#endif // TAG_RECOGNITION_H
