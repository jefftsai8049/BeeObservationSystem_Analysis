#ifndef TAG_RECOGNITION_H
#define TAG_RECOGNITION_H

#define tagSize 24
#define tagBinaryThreshold 110


#include <QObject>
#include <QDebug>
#include <QTime>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QMap>

#include <opencv.hpp>
#include <core/ocl.hpp>


class tag_recognition : public QObject
{
    Q_OBJECT
public:
    explicit tag_recognition(QObject *parent = 0);
    ~tag_recognition();

    void tagImgProc(cv::Mat src, cv::Mat &word1, cv::Mat &word2);

    void wordImage2Data(cv::Mat &src);

    int wordRecognition(cv::Mat &src);

    int wordMapping(const int &result);

    bool loadSVMModel(const std::string &fileName);

    void loadTrainData(const QString &path, cv::Mat &trainData, cv::Mat &trainLabel);

    void loadTestData(const QString &path, std::vector<cv::Mat> &testData, std::vector<int> &testLabel);

    bool loadPCAModel(const std::string &fileName);

signals:

public slots:


private:

    void findBlobs(const cv::Mat binary,std::vector<std::vector<cv::Point2f>> &blobs);

    float calcualteCOV(std::vector<cv::Point2f> points);

    void sortblobs(std::vector<std::vector<cv::Point2f>> &blobs);

    void sortblobsSize(std::vector<std::vector<cv::Point2f>> &blobs);

    std::vector<std::vector<cv::Point2f> > removeImpossibleBlobs(std::vector<std::vector<cv::Point2f> > blobs);

    float findRotateAngle(cv::Point2f circleCenter,cv::Point2f imgCenter);

    float findRotateAngle(std::vector<cv::Point2f> blobsCenter, cv::Point2f &imgCenter);

    cv::Mat drawBlob(std::vector<std::vector<cv::Point2f> > blobs);

    cv::Mat drawBlobMask(std::vector<std::vector<cv::Point2f> > blobs);

    void findBlobCenetr(std::vector<std::vector<cv::Point2f>> blobs,std::vector<cv::Point2f> &blobCenter);

    void cutWords(cv::Mat wordsMask, cv::Mat rawDst, cv::Mat &word1, cv::Mat &word2);

    cv::Ptr<cv::ml::SVM> SVMModel;

    cv::PCA pca;
};

#endif // TAG_RECOGNITION_H
