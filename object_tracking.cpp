#include "object_tracking.h"



int track::size()
{
    return time.size();
}

cv::Point track::lastPosition()
{
    return position[time.size()];
}



object_tracking::object_tracking(QObject *parent)
{

}

object_tracking::~object_tracking()
{

}
