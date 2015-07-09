#include "trajectory_tracking.h"

trajectory_tracking::trajectory_tracking(QObject *parent) : QThread(parent)
{
    frame.resize(3);
}

trajectory_tracking::~trajectory_tracking()
{

}

void trajectory_tracking::setImageShiftOriginPoint(std::vector<cv::Point> originPoint)
{
    this->originPoint = originPoint;

}

cv::Mat trajectory_tracking::imageShift(std::vector<cv::Mat> stitchFrame, std::vector<cv::Point> originPoint)
{

//    qDebug() << this->originPoint[0].x << this->originPoint[0].y << this->originPoint[1].x << this->originPoint[1].y << this->originPoint[2].x << this->originPoint[2].y;
    cv::Mat cat(cv::Size(imgSizeX*3,imgSizeY),CV_8UC3,cv::Scalar(0));
    for (int i=0;i<3;i++)
    {
        if(this->originPoint[i].x>=0&&this->originPoint[i].y>=0)
        {
            stitchFrame[i](cv::Rect(0,0,fmin(imgSizeX,imgSizeX*3-this->originPoint[i].x),imgSizeY-this->originPoint[i].y)).copyTo(cat(cv::Rect(this->originPoint[i].x,this->originPoint[i].y,fmin(imgSizeX,imgSizeX*3-this->originPoint[i].x),imgSizeY-this->originPoint[i].y)));
        }

    }

//    cv::imshow("Stitch",cat);
    return cat;
}

cv::Mat trajectory_tracking::imageShift(std::vector<cv::Mat> stitchFrame)
{
//    qDebug() << this->originPoint[0].x << this->originPoint[0].y << this->originPoint[1].x << this->originPoint[1].y << this->originPoint[2].x << this->originPoint[2].y;
    cv::Mat cat(cv::Size(imgSizeX*3,imgSizeY),CV_8UC3,cv::Scalar(0));
    for (int i=0;i<3;i++)
    {
        if(this->originPoint[i].x>=0&&this->originPoint[i].y>=0)
        {
            stitchFrame[i](cv::Rect(0,0,fmin(imgSizeX,imgSizeX*3-this->originPoint[i].x),imgSizeY-this->originPoint[i].y)).copyTo(cat(cv::Rect(this->originPoint[i].x,this->originPoint[i].y,fmin(imgSizeX,imgSizeX*3-this->originPoint[i].x),imgSizeY-this->originPoint[i].y)));
        }

    }

//    cv::imshow("Stitch",cat);
    return cat;
}

void trajectory_tracking::setVideoName(std::vector<std::string> videoName)
{
    this->videoName = videoName;

}

void trajectory_tracking::stopStitch()
{
    this->stopped = true;
}

void trajectory_tracking::run()
{
    std::vector<cv::VideoCapture> cap(3);

        for(int i = 0; i < 3; i++)
        {
            cap[i].open(this->videoName[i]);
            if(!cap[i].isOpened())
            {
                return;
            }
        }

    this->stopped = false;

    cv::Mat pano;

    while(!this->stopped)
    {
        QTime clock;
        clock.start();
        std::vector<cv::Mat> frame(3);
        for(int i = 0; i < 3; i++)
        {
            cap[i].read(frame[i]);
        }
        if(frame[0].empty()||frame[1].empty()||frame[2].empty())
        {
            this->stopped = true;
            break;
        }
        pano = this->imageShift(frame);
        pano = this->bgr2gray(pano);
//        cv::imshow("Stitch",pano);
//        cv::waitKey(1);
        std::vector<cv::Vec3f> circles;
        int dp = 2;
        double miniDist = 20;
        int para_1 = 80;
        int para_2 = 80;
        int minRadius = 12;
        int maxRadius = 19;
        cv::HoughCircles(pano,circles,CV_HOUGH_GRADIENT,dp,miniDist,para_1,para_2,minRadius,maxRadius);

//        qDebug() << circles.size();
        for(int i = 0; i < circles.size(); i++ )
        {
           cv::Point center(circles[i][0], circles[i][1]);
           int radius = circles[i][2];
           // circle center
    //       circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
           // circle outline
           cv::circle( pano, center, radius, cv::Scalar(255), 1, 8, 0 );
         }
        qDebug() << clock.elapsed();
//        cv::imshow("Stitch",pano);
//        cv::waitKey(3);
    }

    for(int i = 0; i < 3; i++)
    {
        cap[i].release();
    }

}

cv::Mat trajectory_tracking::bgr2gray(cv::Mat src)
{
    cv::Mat dst;
    dst.create(src.rows,src.cols,CV_8UC1);
    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            dst.at<uchar>(i,j) = (src.at<cv::Vec3b>(i,j)[0]+src.at<cv::Vec3b>(i,j)[1]+src.at<cv::Vec3b>(i,j)[2])/3;
        }
    }
    return dst;
}

