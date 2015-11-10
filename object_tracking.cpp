#include "object_tracking.h"



int track::size()
{
    return time.size();
}

cv::Point track::lastPosition()
{
    if(time.size() > 0)
        return position[time.size()-1];
    else
        return  cv::Point(-1,-1);
}

cv::Point track::futurePosition()
{
    if(time.size() > 1)
        return 2*position[time.size()-1]+position[time.size()-2];
    else
        return cv::Point(-1,-1);
}

double track::howClose(const cv::Vec3f &circle)
{
    cv::Point circleCenter(circle[0],circle[1]);
    cv::Point lastPoint = lastPosition();

    if(lastPoint != cv::Point(-1,-1))
    {
        cv::Point distance = circleCenter-lastPoint;
        return pow(distance.x,2)+pow(distance.y,2);

    }
    else
    {
        return NULL;
    }
}


object_tracking::object_tracking(QObject *parent)
{

}

object_tracking::~object_tracking()
{

}

void object_tracking::compute(const QDateTime &time, const std::vector<cv::Vec3f> &circles, const std::vector<std::string> &w1, const std::vector<std::string> &w2)
{
    int circleSize = circles.size();
    int pathSize = this->path.size();
    if(circleSize > pathSize)
    {
        track emptyPath;
        for(int i = 0; i < circleSize-pathSize;i++)
            this->path.push_back(emptyPath);
    }

    double **probability;
    probability = (double**)malloc(sizeof(double*)*pathSize);
    for(int i = 0;i < pathSize; i++)
    {
        probability[i] = (double*)malloc(sizeof(double)*circleSize);
        for(int j = 0;j < circleSize; j++)
        {
            probability[i][j] = this->path[i].howClose(circles[j]);
        }
        double minVal;
        int index;
        mf::vectorFindMin(minVal,index,probability[i]);
        if(minVal < 100)
        {
            this->path[i].time.push_back(time);
            this->path[i].position.push_back(cv::Point(circles[index][0],circles[index][1]));
        }

    }



}

void object_tracking::setImageRange(const cv::Size &imageRange)
{
    this->range = imageRange;
}

void object_tracking::lastPath(std::vector<std::vector<cv::Point> > &path)
{
    path.clear();
    for(int i = 0;i < this->path.size();i++)
    {
        if(this->path[i].size() < 5)
        {
            path.push_back(this->path[i].position);

        }
        else
        {
            std::vector<cv::Point>::const_iterator first = this->path[i].position.end()-5;
            std::vector<cv::Point>::const_iterator last = this->path[i].position.end();
            std::vector<cv::Point> temp(first,last);

            path.push_back(temp);

        }
    }

}

void object_tracking::drawPath(cv::Mat &src, const std::vector<std::vector<cv::Point> > &path)
{
    for(int i = 0; i < path.size();i++)
    {
        for(int j = 0 ;j < path[i].size()-1;j++)
        {
//            if(path[i][j] != cv::Point(-1,-1) || path[i][j+1] != cv::Point(-1,-1))
//                cv::line(src,path[i][j],path[i][j+1],cv::Scalar(255,255,255),4);
        }
    }
}