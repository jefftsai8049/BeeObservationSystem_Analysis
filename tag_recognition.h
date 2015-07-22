#ifndef TAG_RECOGNITION_H
#define TAG_RECOGNITION_H

#define tagSize 24
#define tagBinaryThreshold 110


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

    void findBlobs(const cv::Mat binary,std::vector<std::vector<cv::Point2i>> &blobs);

    float calcualteCOV(std::vector<cv::Point2i> points);

    void sortblobs(std::vector<std::vector<cv::Point2i>> blobs);

    std::vector<std::vector<cv::Point2i>> removeImpossibleBlobs(std::vector<std::vector<cv::Point2i>> blobs);

    float findRotateAngle(cv::Point circleCenter,cv::Point imgCenter);

    cv::Mat drawBlob(std::vector<std::vector<cv::Point2i>> blobs);
};

#endif // TAG_RECOGNITION_H
