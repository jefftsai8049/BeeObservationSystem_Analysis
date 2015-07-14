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
    return cat;
}

cv::Mat trajectory_tracking::imageShift(std::vector<cv::Mat> stitchFrame)
{
    //    qDebug() << this->originPoint[0].x << this->originPoint[0].y << this->originPoint[1].x << this->originPoint[1].y << this->originPoint[2].x << this->originPoint[2].y;
    cv::Mat cat(cv::Size(imgSizeX*3,imgSizeY),CV_8UC1,cv::Scalar(0));

    if(stitchFrame[0].type() == CV_8UC3)
    {
        cat.convertTo(cat,CV_8UC3);
    }


    for (int i=0;i<3;i++)
    {
        if(this->originPoint[i].x>=0&&this->originPoint[i].y>=0)
        {
            stitchFrame[i](cv::Rect(0,0,fmin(imgSizeX,imgSizeX*3-this->originPoint[i].x),imgSizeY-this->originPoint[i].y)).copyTo(cat(cv::Rect(this->originPoint[i].x,this->originPoint[i].y,fmin(imgSizeX,imgSizeX*3-this->originPoint[i].x),imgSizeY-this->originPoint[i].y)));
        }

    }
    return cat;
}

void trajectory_tracking::setVideoName(std::vector<std::string> videoName)
{
    this->videoName = videoName;

}

void trajectory_tracking::setHoughCircleParameters(const int &dp,const int &miniDist,const int &para_1,const int &para_2,const int &minRadius,const int &maxRadius)
{
    this->dp = dp;

    this->miniDist = miniDist;

    this->para_1 = para_1;

    this->para_2 = para_2;

    this->minRadius = minRadius;

    this->maxRadius = maxRadius;
}

void trajectory_tracking::setShowImage(const bool &status)
{
    showImage = status;
}

void trajectory_tracking::stopStitch()
{
    this->stopped = true;
}

void trajectory_tracking::run()
{
    //open video file
    std::vector<cv::VideoCapture> cap(3);
    for(int i = 0; i < 3; i++)
    {
        cap[i].open(this->videoName[i]);
        if(!cap[i].isOpened())
        {
            return;
        }
    }
    //thread stop flag
    this->stopped = false;

    cv::Mat pano;

    //main processing loop
    while(!this->stopped)
    {
        //calculate fps
        QTime clock;
        clock.start();
        //capture frame and convert to gray
        std::vector<cv::Mat> frame(3);
        std::vector<cv::Mat> frameGray(3);
        for(int i = 0; i < 3; i++)
        {
            cap[i].read(frame[i]);
            frameGray[i] = this->bgr2gray(frame[i]);
        }
        if(frame[0].empty()||frame[1].empty()||frame[2].empty())
        {
            this->stopped = true;
            break;
        }
        //stitching image
        pano = this->imageShift(frameGray);

        //hough circle detection
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(pano,circles,CV_HOUGH_GRADIENT,dp,miniDist,para_1,para_2,minRadius,maxRadius);
        if (showImage)
        {
            //draw cicle
            cv::Mat panoDrawCircle = pano.clone();
            for(int i = 0; i < circles.size(); i++ )
            {
                cv::Point center(circles[i][0], circles[i][1]);
                int radius = circles[i][2];
                cv::circle( panoDrawCircle, center, radius, cv::Scalar(255), 1, 8, 0 );

            }
            cv::resize(panoDrawCircle,panoDrawCircle,cv::Size(panoDrawCircle.cols/2,panoDrawCircle.rows/2));
            //show image
            cv::imshow("Stitch",panoDrawCircle);
            cv::waitKey(3);
        }
        //cut subimage
        std::vector<cv::Mat> circleImg(circles.size());
        for (int i=0;i<circles.size();i++)
        {
            cv::getRectSubPix(pano,cv::Size(circles[i][2]*2-1,circles[i][2]*2-1),cv::Point(circles[i][0], circles[i][1]),circleImg[i]);
            cv::imshow("tag",circleImg[i]);
            cv::waitKey(3);
        }


        emit sendFPS(1000.0/clock.elapsed());
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

