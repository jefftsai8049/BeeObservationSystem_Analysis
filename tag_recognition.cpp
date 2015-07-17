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
    blobParams.minDistBetweenBlobs = 0;
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
    cv::imshow("tagBinary",srcBinary);
    if(keypoints.size() != 3)
    {
        cv::Mat black(tagSize,tagSize,CV_8UC1,cv::Scalar::all(0));
        return black;
    }

    std::vector<int> blobPointIndex(3);
    blobPointIndex = {0,1,2};
    std::vector<cv::Point> blobPoint(3);
    for (int i =0;i<keypoints.size(); i++)
    {
        bool totalStatus = 1;
        for (int j =i;j<keypoints.size(); j++)
        {
            std::vector<bool> status(3);

            //改用covaiance?
            if (keypoints[i].size>keypoints[j].size)
            {
                status[j] = 1;
                int temp = blobPointIndex[i];
                blobPointIndex[i] = blobPointIndex[j];
                blobPointIndex[j] = temp;
            }
            else
                status[j] = 0;
            totalStatus *= status[j];
        }
    }


    for (int i=0; i<keypoints.size(); i++)
    {
//        qDebug() << blobPointIndex[i] << keypoints[blobPointIndex[i]].size << keypoints[blobPointIndex[i]].pt.x <<keypoints[blobPointIndex[i]].pt.y;
        cv::circle(src,cv::Point(keypoints[i].pt.x,keypoints[i].pt.y),2,cv::Scalar(255),-1);
    }
    cv::Point topPoint,sidePoint1,sidePoint2;
    topPoint = cv::Point(keypoints[blobPointIndex[0]].pt.x,keypoints[blobPointIndex[0]].pt.y);
    sidePoint1 = cv::Point(keypoints[blobPointIndex[1]].pt.x,keypoints[blobPointIndex[1]].pt.y);
    sidePoint2 = cv::Point(keypoints[blobPointIndex[2]].pt.x,keypoints[blobPointIndex[2]].pt.y);

    cv::Point circleCenter;
    circleCenter = (sidePoint1+sidePoint2)/2;

    qDebug() << topPoint.x <<topPoint.y << circleCenter.x <<circleCenter.y;
    cv::circle(src,cv::Point(topPoint.x,topPoint.y),2,cv::Scalar(0),-1);
    cv::circle(src,cv::Point(circleCenter.x,circleCenter.y),2,cv::Scalar(0),-1);
    return src;
}

cv::Mat tag_recognition::calcualteCOV(cv::KeyPoint points)
{

}

