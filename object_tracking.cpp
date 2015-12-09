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
        return 2*position[time.size()-1]-position[time.size()-2];
    else
        return cv::Point(-1,-1);
}

double track::howClose(const cv::Vec3f &circle)
{
    cv::Point circleCenter(circle[0],circle[1]);
    cv::Point lastPoint = lastPosition();
    cv::Point futurePoint = futurePosition();

    if(lastPoint != cv::Point(-1,-1))
    {
        cv::Point distance = circleCenter-lastPoint;
        cv::Point predictDistance = circleCenter-futurePoint;
        return fmin(pow(distance.x,2)+pow(distance.y,2),pow(predictDistance.x,2)+pow(predictDistance.y,2));

    }
    else
    {
        return NULL;
    }
}



object_tracking::object_tracking(QObject *parent, const QDateTime fileTime)
{
    this->startTime = fileTime;
}

object_tracking::~object_tracking()
{

}

void object_tracking::compute(const QDateTime &time, const std::vector<cv::Vec3f> &circles, const std::vector<std::string> &w1, const std::vector<std::string> &w2)
{

    this->nowTime = time;

    //threshold of distance
    int minDistanceThreshold = 2000;
    //    int minTimeGap = 5;

    //for saving path and circle size
    int circleSize = circles.size();
    int pathSize = this->path.size();
    std::vector<int> circleMarked(circleSize);

    //-1-for not belong any path
    //0+-for belong which path
    for(int i = 0; i < circleSize; i++)
    {
        circleMarked[i] = -1;
    }

    //check path and circle size
    for(int j = 0; j < pathSize; j++)
    {
        //calculate each circle to each path distance
        std::vector<double> distanceSqrt(circleSize);
        for(int k = 0; k < circleSize; k++)
        {
            distanceSqrt[k] = this->path[j].howClose(circles[k]);
        }
        //find the min distance of circle to lastapth
        double minDiatanceSqrt;
        int index;
        mf::vectorFindMin(minDiatanceSqrt,index,distanceSqrt);

        int timeGap = this->path[j].time[this->path[j].time.size()-1].secsTo(time);


#ifdef DEBUG_OBJECT_TRACKING
        qDebug() <<"path:" << j << "circle:" << index << "min distance " << minDiatanceSqrt;
#endif
        //if distance and time is close enough
        if(minDiatanceSqrt < minDistanceThreshold)
        {
            if(timeGap < FORGET_TRACKING_TIME)
            {
                this->path[j].time.push_back(time);
                this->path[j].w1.push_back(w1[index].c_str()[0]);
                this->path[j].w2.push_back(w2[index].c_str()[0]);
                this->path[j].position.push_back(cv::Point(circles[index][0],circles[index][1]));
                circleMarked[index] = j;
            }
        }

    }

    int count = 0;
    for(int k = 0; k < circleSize; k++)
    {

        if(circleMarked[k] == -1)
        {
            //qDebug() << "new path" << count+pathSize;
            track emptyPath;
            emptyPath.time.push_back(time);
            emptyPath.w1.push_back(w1[k].c_str()[0]);
            emptyPath.w2.push_back(w2[k].c_str()[0]);
            emptyPath.position.push_back(cv::Point(circles[k][0],circles[k][1]));

            circleMarked[k] = count+pathSize;
            count++;

            this->path.push_back(emptyPath);
        }
    }

    //    for(int i = 0; i < this->path.size(); i++)
    //    {
    //        qDebug() << "pathSize" << this->path.size() << "position" << this->path[i].position[0].x << this->path[i].position[0].y;
    //    }

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
        if((this->path[i].time[this->path[i].time.size()-1].secsTo(nowTime)) < 5)
        {

            if(this->path[i].size() < REMAIN_SIZE)
            {
                path.push_back(this->path[i].position);

            }
            else
            {
                std::vector<cv::Point>::const_iterator first = this->path[i].position.end()-REMAIN_SIZE;
                std::vector<cv::Point>::const_iterator last = this->path[i].position.end();
                std::vector<cv::Point> temp(first,last);
                path.push_back(temp);

            }
        }
    }

}

void object_tracking::drawPath(cv::Mat& src)
{

    std::vector<std::vector<cv::Point> > path;
    this->lastPath(path);
    //qDebug() << "i" << path.size();

    for(int i = 0; i < path.size();i++)
    {
        //qDebug() <<"i" << i << "j" << path[i].size();
        if(path[i].size() < 1)
        {
            //qDebug() << "size not enough";
            break;
        }
        for(int j = 0 ;j < path[i].size()-1;j++)
        {

            //qDebug() << path[i][j].x <<path[i][j].y << path[i][j+1].x <<path[i][j+1].y;
            if(path[i][j] != cv::Point(-1,-1) || path[i][j+1] != cv::Point(-1,-1))
                cv::line(src,path[i][j],path[i][j+1],cv::Scalar(255,255,255),4);
        }
    }
}

void object_tracking::savePath()
{

    QFileInfo fileInfo(startTime.toString("yyyy-MM-dd_hh-mm-ss-zzz")+".csv");
    QFile file(fileInfo.fileName());

    if(!fileInfo.exists())
    {
        qDebug() << this->startTime.toString("yyyy-MM-dd_hh-mm-ss-zzz")+".csv";

        file.open(QIODevice::WriteOnly);
        qDebug() <<fileInfo.exists();
    }
    else
    {
        file.open(QIODevice::Append);
    }
    std::stringstream outFile;
    for(int i = 0; i < this->path.size(); i++)
    {
        //qDebug() << this->path[i].time[this->path[i].time.size()-1].secsTo(this->nowTime);
        if(this->path[i].time[this->path[i].time.size()-1].secsTo(this->nowTime) > FORGET_TRACKING_TIME)
        {
            if(this->path[i].time.size() > SHORTEST_SAMPLE_SIZE)
            {
                for(int j = 0; j < this->path[i].time.size(); j++)
                {
                    //                std::vector<std::vector<char>> name;
                    //                std::vector<cv::Point> position;
                    //                std::vector<QDateTime> time;
                    outFile << this->path[i].w1[j] << "," << this->path[i].w2[j] << ","
                            << this->path[i].time[j].toString("yyyy-MM-dd_hh-mm-ss-zzz").toStdString() << ","
                            << this->path[i].position[j].x << ","
                            << this->path[i].position[j].y << ",";
                }
                outFile << "\n";
            }
            this->path.erase(this->path.begin()+i);
            i--;
        }
    }
    file.write(outFile.str().c_str());
    file.close();
}

void object_tracking::saveAllPath()
{
    QFileInfo fileInfo(startTime.toString("yyyy-MM-dd_hh-mm-ss-zzz")+".csv");
    QFile file(fileInfo.fileName());

    if(!fileInfo.exists())
    {
        qDebug() << this->startTime.toString("yyyy-MM-dd_hh-mm-ss-zzz")+".csv";

        file.open(QIODevice::WriteOnly);
        qDebug() <<fileInfo.exists();
    }
    else
    {
        file.open(QIODevice::Append);
    }
    std::stringstream outFile;
    for(int i = 0; i < this->path.size(); i++)
    {
        //qDebug() << this->path[i].time[this->path[i].time.size()-1].secsTo(this->nowTime);
        if(this->path[i].time.size() > SHORTEST_SAMPLE_SIZE)
        {
            for(int j = 0; j < this->path[i].time.size(); j++)
            {
                //                std::vector<std::vector<char>> name;
                //                std::vector<cv::Point> position;
                //                std::vector<QDateTime> time;
                outFile << this->path[i].w1[j] << "," << this->path[i].w2[j] << ","
                        << this->path[i].time[j].toString("yyyy-MM-dd_hh-mm-ss-zzz").toStdString() << ","
                        << this->path[i].position[j].x << ","
                        << this->path[i].position[j].y << ",";
            }
            outFile << "\n";
            //            this->path.erase(this->path.begin()+i);
            //            i--;
        }
    }
    this->path.clear();
    file.write(outFile.str().c_str());
    file.close();
}


