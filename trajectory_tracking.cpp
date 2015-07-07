#include "trajectory_tracking.h"

trajectory_tracking::trajectory_tracking(QObject *parent) : QObject(parent)
{


}

trajectory_tracking::~trajectory_tracking()
{

}

void trajectory_tracking::imageShift(std::vector<cv::Mat> stitchFrame, std::vector<cv::Point> originPoint)
{
    qDebug() << originPoint[0].x << originPoint[0].y << originPoint[1].x << originPoint[1].y << originPoint[2].x << originPoint[2].y;
    cv::Mat cat(cv::Size(imgSizeX*3,imgSizeY),CV_8UC3,cv::Scalar(0));
    for (int i=0;i<3;i++)
    {
        if(originPoint[i].x>0&&originPoint[i].y>0)
        {
            stitchFrame[i](cv::Rect(0,0,fmin(imgSizeX,imgSizeX*3-originPoint[i].x),imgSizeY-originPoint[i].y)).copyTo(cat(cv::Rect(originPoint[i].x,originPoint[i].y,fmin(imgSizeX,imgSizeX*3-originPoint[i].x),imgSizeY-originPoint[i].y)));
        }

    }
    cv::imshow("Stitch",cat);
    //    if (shiftDelta[0] > 1200 || shiftDelta[0] < -1200)
    //    {
    //        qDebug() << "fuck";
    //        return;
    //    }
    //    if (shiftDelta[0] > 0 && shiftDelta[1] > 0)
    //    {
    //        cv::Mat cat(cv::Size(3600,1600),CV_8U,cv::Scalar::all(0));
    //        stitchFrame[1](cv::Rect(shiftDelta[0],shiftDelta[1],stitchFrame[1].cols-shiftDelta[0],stitchFrame[1].rows-shiftDelta[1])).copyTo(stitchFrame[1](cv::Rect(0,0,stitchFrame[1].cols-shiftDelta[0],stitchFrame[1].rows-shiftDelta[1])));
    //        cv::hconcat(stitchFrame,cat);
    //        cv::imshow("Stitch",cat);
    //    }
}

