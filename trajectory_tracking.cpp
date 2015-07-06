#include "trajectory_tracking.h"

trajectory_tracking::trajectory_tracking(QObject *parent) : QObject(parent)
{

}

trajectory_tracking::~trajectory_tracking()
{

}

void trajectory_tracking::imageShift(std::vector<cv::Mat> stitchFrame, std::vector<int> shiftDelta)
{
    qDebug () << shiftDelta[0] << shiftDelta[1];
    if (shiftDelta[0] > 1200 || shiftDelta[0] < -1200)
    {
        qDebug() << "fuck";
        return;
    }
    if (shiftDelta[0] > 0 && shiftDelta[1] > 0)
    {
        cv::Mat cat(cv::Size(3600,1600),CV_8U,cv::Scalar::all(0));
        stitchFrame[1](cv::Rect(shiftDelta[0],shiftDelta[1],stitchFrame[1].cols-shiftDelta[0],stitchFrame[1].rows-shiftDelta[1])).copyTo(stitchFrame[1](cv::Rect(0,0,stitchFrame[1].cols-shiftDelta[0],stitchFrame[1].rows-shiftDelta[1])));
        cv::hconcat(stitchFrame,cat);
        cv::imshow("Stitch",cat);
    }
}

