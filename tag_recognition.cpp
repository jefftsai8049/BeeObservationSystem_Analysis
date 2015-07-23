#include "tag_recognition.h"

tag_recognition::tag_recognition(QObject *parent) : QObject(parent)
{

}

tag_recognition::~tag_recognition()
{

}

void tag_recognition::tagImgProc(cv::Mat src,cv::Mat &word1,cv::Mat &word2)
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

    cv::imshow("Binary",srcBinary);

    //normalize from 0-255 to 0-1
    cv::Mat srcBinaryZeroOne;
    cv::normalize(srcBinary,srcBinaryZeroOne,0,1,cv::NORM_MINMAX);

    //find white blobs
    std::vector < std::vector<cv::Point2f> > blobs;
    this->findBlobs(srcBinaryZeroOne,blobs);

    //sort the blobs area by cov
    this->sortblobs(blobs);

    //remove those impossible blobs
    blobs = this->removeImpossibleBlobs(blobs);

    this->sortblobsSize(blobs);

    std::vector<cv::Point2f> blobCenter(3);
    for(int i = 0; i < blobs.size(); i++)
    {
        blobCenter[i] = cv::Point(0,0);
        for(int j=0; j < blobs[i].size(); j++)
        {
            int x = blobs[i][j].x;
            int y = blobs[i][j].y;
            blobCenter[i].x += (float)x;
            blobCenter[i].y += (float)y;
        }
        blobCenter[i].x = blobCenter[i].x/(float)blobs[i].size();
        blobCenter[i].y = blobCenter[i].y/(float)blobs[i].size();
        qDebug() << i << blobs[i].size() << blobCenter[i].x << blobCenter[i].y << this->calcualteCOV(blobs[i]);
    }


    //draw blobs

    cv::imshow("blobs",this->drawBlob(blobs));

    float angle = this->findRotateAngle(blobCenter[0],cv::Point(tagSize/2,tagSize/2));
    cv::Point2f imgCenter = cv::Point2f(tagSize/2.0,tagSize/2.0);
    //    float angle = this->findRotateAngle(blobCenter,imgCenter);
    //    qDebug() << "center" << imgCenter.x << imgCenter.y;
    cv::Mat rotateInfo = cv::getRotationMatrix2D(imgCenter, -(angle-90), 1.0);
    //    cv::Mat rotateInfo = cv::getRotationMatrix2D(imgCenter, angle, 1.0);
    cv::warpAffine(srcNoCircle,srcNoCircle,rotateInfo,srcNoCircle.size(),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(255));
    cv::Mat wordsMask;
    cv::warpAffine(this->drawBlobMask(blobs),wordsMask,rotateInfo,cv::Size(tagSize,tagSize),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(0));
    cv::imshow("mask",wordsMask);


    cv::Mat rawDst(tagSize,tagSize,CV_8UC1,cv::Scalar::all(255));

    srcNoCircle.copyTo(rawDst,wordsMask);
    cv::imshow("dst",rawDst);

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat binaryDst;
    int thresh = 100;
    cv::threshold(rawDst,binaryDst,thresh,255,cv::THRESH_BINARY);
    cv::findContours( binaryDst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );


    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect( contours.size() );

    //    std::vector<cv::Mat> dst(2);
    cv::Mat dst;

    for( int i = 0; i < contours.size(); i++ )
    {
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
        cv::Point br = boundRect[i].br();
        cv::Point tl = boundRect[i].tl();

        cv::getRectSubPix(rawDst,cv::Size(br.x-tl.x,br.y-tl.y),(br+tl)/2,dst);
        //        dstVec[i] = dst.clone();
//        cv::imshow("dst",dst);
        qDebug() << i;
        if(i == 0)
        {
            word1 = dst.clone();
            cv::imshow("word1",word1);
        }
        else if(i == 2)
        {
            word2 = dst.clone();
            cv::imshow("word2",word2);
        }
    }

}

void tag_recognition::findBlobs(const cv::Mat binary, std::vector<std::vector<cv::Point2f> > &blobs)
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

            std::vector <cv::Point2f> blob;

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

float tag_recognition::calcualteCOV(std::vector<cv::Point2f> points)
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

void tag_recognition::sortblobs(std::vector<std::vector<cv::Point2f>> &blobs)
{
    for (int i = 0;i < blobs.size()-1; i++)
    {
        for(int j = i+1;j < blobs.size(); j++)
        {
            if(this->calcualteCOV(blobs[i]) > this->calcualteCOV(blobs[j]))
            {
                std::vector<cv::Point2f> temp = blobs[i];
                blobs[i] = blobs[j];
                blobs[j] = temp;
            }
        }
    }
}

void tag_recognition::sortblobsSize(std::vector<std::vector<cv::Point2f> > &blobs)
{
    for (int i = 0;i < blobs.size()-1; i++)
    {
        for(int j = i+1;j < blobs.size(); j++)
        {
            if(blobs[i].size() > blobs[j].size())
            {
                std::vector<cv::Point2f> temp = blobs[i];
                blobs[i] = blobs[j];
                blobs[j] = temp;
            }
        }
    }
}

std::vector<std::vector<cv::Point2f> > tag_recognition::removeImpossibleBlobs(std::vector<std::vector<cv::Point2f> > blobs)
{
    for(int i = 0; i < blobs.size(); i++)
    {
        //        qDebug() << i << blobs[i].size() << this->calcualteCOV(blobs[i]) <<this->calcualteCOV(blobs[i])*blobs[i].size();
        if(blobs[i].size() < 5 || i > 2)
        {
            blobs.erase(blobs.begin()+i);
            i--;
        }
    }
    return blobs;
}

float tag_recognition::findRotateAngle(cv::Point2f circleCenter, cv::Point2f imgCenter)
{
    float angle;
    float x,y,r;
    x = circleCenter.x-imgCenter.x;
    y = -(circleCenter.y-imgCenter.y);
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
    //    qDebug() << angle;
    //    cv::imshow("labelled", output);
    return angle;
}

float tag_recognition::findRotateAngle(std::vector<cv::Point2f> blobsCenter, cv::Point2f &imgCenter)
{
    float angle;

    //    cv::Point2f wordsCenter;
    imgCenter = (blobsCenter[1]+blobsCenter[2])/2.0;
    //    qDebug() << blobsCenter[1].x << blobsCenter[1].y << blobsCenter[2].x << blobsCenter[2].y << imgCenter.x <<imgCenter.y;

    std::vector<cv::Point2f> vec2Center(3);
    for(int i=0;i<blobsCenter.size();i++)
    {
        blobsCenter[i].y = imgCenter.y-blobsCenter[i].y;
        vec2Center[i] = blobsCenter[i]-imgCenter;
    }
    cv::Point2f vecAngle;
    vecAngle = vec2Center[1]-vec2Center[0];
    if(vec2Center[0].y <= 0)
    {
        angle = (atan(vecAngle.y/vecAngle.x)/(2.0*3.1415926)*360.0-90.0);
    }
    else
    {
        angle = (atan(vecAngle.y/vecAngle.x)/(2.0*3.1415926)*360.0-90.0);
    }

    //    qDebug() << angle;
    return angle;
}

cv::Mat tag_recognition::drawBlob(std::vector<std::vector<cv::Point2f> > blobs)
{
    cv::Mat output = cv::Mat::zeros(cv::Size(tagSize,tagSize), CV_8UC3);
    for(int i = 0;i<blobs.size();i++)
    {
        if(i!=0)
            break;
        unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
        unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
        unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));
        for(int j=0; j < blobs[i].size(); j++)
        {
            int x = blobs[i][j].x;
            int y = blobs[i][j].y;
            output.at<cv::Vec3b>(y,x)[0] = b;
            output.at<cv::Vec3b>(y,x)[1] = g;
            output.at<cv::Vec3b>(y,x)[2] = r;
        }
    }
    return output;
}

cv::Mat tag_recognition::drawBlobMask(std::vector<std::vector<cv::Point2f> > blobs)
{
    cv::Mat output = cv::Mat::zeros(cv::Size(tagSize,tagSize), CV_8UC1);
    for(int i = 0;i<blobs.size();i++)
    {
        if(i!=0)
        {
            for(int j=0; j < blobs[i].size(); j++)
            {
                int x = blobs[i][j].x;
                int y = blobs[i][j].y;
                output.at<uchar>(y,x) = 255;
            }
        }
    }
    return output;
}

