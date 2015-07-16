#include "tag_recognition.h"

tag_recognition::tag_recognition(QObject *parent) : QObject(parent)
{

}

tag_recognition::~tag_recognition()
{

}

cv::Mat tag_recognition::tagImgProc(cv::Mat src)
{
    cv::resize(src,src,cv::Size(tagSize,tagSize));
    cv::equalizeHist(src,src);
    cv::Mat circleMask(tagSize,tagSize,CV_8UC1,cv::Scalar::all(0));

    cv::circle(circleMask,cv::Point2f(12,12),11,cv::Scalar(255),-1);
    cv::Mat srcNoCircle(tagSize,tagSize,CV_8UC1,cv::Scalar::all(255));
    src.copyTo(srcNoCircle,circleMask);

    cv::Mat srcBinary;
    cv::threshold(srcNoCircle,srcBinary,tagBinaryThreshold,255,CV_THRESH_BINARY_INV);

    cv::SimpleBlobDetector::Params blobParams;
    blobParams.minDistBetweenBlobs = 1.0;
    blobParams.filterByInertia = true;
    blobParams.minInertiaRatio = 0.1;
    blobParams.maxInertiaRatio = 1.0;
    blobParams.filterByConvexity = false;
    blobParams.filterByColor = false;
    blobParams.filterByCircularity = false;
    blobParams.filterByArea = false;
    cv::Ptr<cv::SimpleBlobDetector> blob = cv::SimpleBlobDetector::create(blobParams);
    std::vector<cv::KeyPoint> keypoints;
    blob->detect(srcBinary, keypoints);

    if(keypoints.size() != 3)
    {
        cv::Mat black(tagSize,tagSize,CV_8UC1,cv::Scalar::all(0));
        return black;
    }
    for (int i=0; i<keypoints.size(); i++){
        cv::circle(src,cv::Point(keypoints[i].pt.x,keypoints[i].pt.y),2,cv::Scalar(255),-1);
    }
    return src;
}

