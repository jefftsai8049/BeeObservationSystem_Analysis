#include "tag_recognition.h"

tag_recognition::tag_recognition(QObject *parent) : QObject(parent)
{

}

tag_recognition::~tag_recognition()
{

}

cv::Mat tag_recognition::tagImgProc(cv::Mat src)
{
    //resize and histogram equalize
    cv::resize(src,src,cv::Size(tagSize,tagSize));
    cv::equalizeHist(src,src);
    cv::Mat circleMask(tagSize,tagSize,CV_8UC1,cv::Scalar::all(0));

    //remove the black circle
    cv::circle(circleMask,cv::Point2f(12,12),11,cv::Scalar(255),-1);
    cv::Mat srcNoCircle(tagSize,tagSize,CV_8UC1,cv::Scalar::all(255));
    src.copyTo(srcNoCircle,circleMask);

    //convert to binary image
    cv::Mat srcBinary;
    cv::threshold(srcNoCircle,srcBinary,tagBinaryThreshold,255,CV_THRESH_BINARY_INV);

    //normalize from 0-255 to 0-1
    cv::Mat srcBinaryZeroOne;
    cv::normalize(srcBinary,srcBinaryZeroOne,0,1,cv::NORM_MINMAX);

    //find white blobs
    std::vector < std::vector<cv::Point2i> > blobs;
    this->findBlobs(srcBinaryZeroOne,blobs);

    //sort the blobs area by cov*size
    for (int i = 0;i < blobs.size()-1; i++)
    {
        for(int j = i+1;j < blobs.size(); j++)
        {
            if(this->calcualteCOV(blobs[i])*blobs[i].size() > this->calcualteCOV(blobs[j])*blobs[j].size())
            {
                std::vector<cv::Point2i> temp = blobs[i];
                blobs[i] = blobs[j];
                blobs[j] = temp;
            }
        }
    }

    //remove those impossible blobs
    //    for(int i = 0; i < blobs.size(); i++)
    //    {
    //        qDebug() << i << blobs[i].size() << this->calcualteCOV(blobs[i]) <<this->calcualteCOV(blobs[i])*blobs[i].size();
    //        if(blobs[i].size() < 8 || this->calcualteCOV(blobs[i])*blobs[i].size() < 100)
    //        {
    //            blobs.erase(blobs.begin()+i);
    //            i--;
    //        }
    //    }

    for(int i = 0; i < blobs.size(); i++)
    {
        qDebug() << i << blobs[i].size() << this->calcualteCOV(blobs[i]) <<this->calcualteCOV(blobs[i])*blobs[i].size();
    }
    //    qDebug() << blobs.size();
    cv::Mat output = cv::Mat::zeros(src.size(), CV_8UC3);


    //draw blobs
    float angle = 0;
    if(blobs.size() > 2)
    {
        cv::Point circleCenter = cv::Point(0,0);
        for(int i = 0;i<blobs.size();i++)
        {
            if(blobs[i].size() < 30 &&blobs[i].size() > 8)
            {
                unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
                unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
                unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));
                //                qDebug() << i << blobs[i].size() <<this->calcualteCOV(blobs[i]) <<this->calcualteCOV(blobs[i])*blobs[i].size();


                for(int j=0; j < blobs[i].size(); j++)
                {
                    int x = blobs[i][j].x;
                    int y = blobs[i][j].y;

                    output.at<cv::Vec3b>(y,x)[0] = b;
                    output.at<cv::Vec3b>(y,x)[1] = g;
                    output.at<cv::Vec3b>(y,x)[2] = r;
                    circleCenter.x += x;
                    circleCenter.y += y;
                }
                circleCenter.x = circleCenter.x/(float)blobs[i].size();
                circleCenter.y = circleCenter.y/(float)blobs[i].size();


                //                qDebug() << atan((center.y/(float)blobs[i].size()-12.0)/(center.x/(float)blobs[i].size()-12.0))/(2.0*3.1415926)*360.0;
                //                qDebug() << atan((float)(center.y/blobs[i].size()-12)/(float)(center.x/blobs[i].size()-12))/*/(2.0*3.1415926)*360.0*/;
                break;
            }

        }
        float x,y,r;
        x = circleCenter.x-tagSize/2.0;
        y = -(circleCenter.y-tagSize/2.0);
        r = sqrt(pow(x,2)+pow(y,2));

//        qDebug() << x << y << r;
        float sinVal = y/r;
        float cosVal = x/r;
        float asinVal = asin(sinVal)/(2.0*3.1415926)*360.0;
        float acosVal = acos(cosVal)/(2.0*3.1415926)*360.0;

//        qDebug() << sinVal << cosVal;
//        qDebug() << asinVal << acosVal;
        //I
        if(sinVal > 0 && cosVal > 0)
        {
            angle = asinVal;
        }
        //II
        else if(sinVal > 0 && cosVal < 0)
        {
            angle = acosVal;
        }
        //III
        else if(sinVal < 0 && cosVal < 0)
        {
            angle = -acosVal;
        }
        //IV
        else if(sinVal < 0 && cosVal > 0)
        {
            angle = asinVal;
        }
        //x-axis right direction
        else if(sinVal == 0 && cosVal > 0)
        {
            angle = 0;
        }
        //x-axis left direction
        else if(sinVal == 0 && cosVal < 0)
        {
            angle = 180;
        }
        //y-axis up direction
        else if(sinVal > 0 && cosVal == 0)
        {
            angle = 90;
        }
        //y-axis down direction
        else if(sinVal > 0 && cosVal == 0)
        {
            angle = -90;
        }
        else
        {
            angle = 0;
        }
        qDebug() << angle;
        cv::imshow("labelled", output);
    }
    cv::Mat rotateInfo = cv::getRotationMatrix2D(cv::Point(tagSize/2,tagSize/2), -(angle-90), 1.0);
    cv::warpAffine(srcNoCircle,srcNoCircle,rotateInfo,srcNoCircle.size());
    return srcNoCircle;
}

void tag_recognition::findBlobs(const cv::Mat binary, std::vector<std::vector<cv::Point2i> > &blobs)
{
    blobs.clear();

    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

    cv::Mat label_image;
    binary.convertTo(label_image, CV_32SC1);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 1) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point2i> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }
    //    qDebug() << blobs.size();
}

float tag_recognition::calcualteCOV(std::vector<cv::Point2i> points)
{

    if(points.size() < 2)
        return NULL;
    float meanX = 0;
    float meanY = 0;
    for(int i = 0; i < points.size(); i++)
    {
        meanX += points[i].x/(float)points.size();
        meanY += points[i].y/(float)points.size();
    }
    //    qDebug() << meanX << meanY;
    float cov = 0;
    for(int i = 0; i < points.size(); i++)
    {

        cov += ((points[i].x-meanX)*(points[i].y-meanY))/(points.size()-1);

    }
    return abs(cov);
}

