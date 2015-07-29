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

    //sort the blobs again by sizes
    this->sortblobsSize(blobs);

    if(blobs.size()<3)
    {
        word1 = cv::Mat::zeros(1,1,CV_8UC1);
        word2 = cv::Mat::zeros(1,1,CV_8UC1);
        return;
    }
    //find blobs center
    std::vector<cv::Point2f> blobCenter;
    this->findBlobCenetr(blobs,blobCenter);

    //    for(int i = 0; i < blobs.size(); i++)
    //    {
    //        qDebug() << i << blobs[i].size() << blobCenter[i].x << blobCenter[i].y << this->calcualteCOV(blobs[i]);
    //    }


    //draw blobs
    cv::imshow("blobs",this->drawBlob(blobs));


    //    cv::Point2f imgCenter = cv::Point2f(tagSize/2.0,tagSize/2.0);

    cv::Point2f imgCenter = (blobCenter[1]+blobCenter[2])/2;

    float angle = this->findRotateAngle(blobCenter[0],imgCenter);

    cv::Mat rotateInfo = cv::getRotationMatrix2D(imgCenter, -(angle-90), 1.0);
    cv::warpAffine(srcNoCircle,srcNoCircle,rotateInfo,srcNoCircle.size(),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(255));
    cv::Mat wordsMask;
    cv::warpAffine(this->drawBlobMask(blobs),wordsMask,rotateInfo,cv::Size(tagSize,tagSize),cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar(0));
    cv::imshow("mask",wordsMask);

    cv::Mat rawDst(tagSize,tagSize,CV_8UC1,cv::Scalar::all(255));

    srcNoCircle.copyTo(rawDst,wordsMask);
    //    cv::imshow("dst",rawDst);
    this->cutWords(wordsMask,rawDst,word1,word2);
}

char tag_recognition::wordRecognition(cv::Mat &src)
{
    QTime t;
    t.start();
    //check input image
    if((src.cols == 1 && src.rows == 1) || (src.cols > 18 || src.rows > 20) || (float)src.rows/(float)src.cols < 1.0 )
    {
//        qDebug() << "shit word" << src.cols << src.rows;
        return '!';
    }
    else
    {
        cv::imshow("src",src);

        int leftPadding = round((src.rows-src.cols)/2.0);
        int rightPadding = floor((src.rows-src.cols)/2.0);

        cv::copyMakeBorder(src,src,0,0,leftPadding,rightPadding,cv::BORDER_CONSTANT,cv::Scalar(255));
        cv::resize(src,src,cv::Size(16,16));
        cv::equalizeHist(src,src);
//        cv::imshow("padding",src);


        src = src.reshape(1,1);
        cv::normalize(src,src,0,1,cv::NORM_MINMAX);
        src.convertTo(src,CV_32FC1);


//        SVMModel = cv::ml::StatModel::load<cv::ml::SVM>("svm_grid_search_opt.yaml");

        char result = SVMModel->predict(src);
//        qDebug() << "result" << result;
        return result;
    }
    //        cv::waitKey(500);
}

bool tag_recognition::loadSVMModel(const std::string &fileName)
{


    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(fileName));
    if(fileInfo.exists())
    {
        //    SVMModel = cv::ml::StatModel::load<cv::ml::SVM>("svm_grid_search_opt.yaml");

        SVMModel = cv::ml::StatModel::load<cv::ml::SVM>(fileName);
        return true;

    }
    else
    {
        return false;
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

void tag_recognition::findBlobCenetr(std::vector<std::vector<cv::Point2f>> blobs,std::vector<cv::Point2f> &blobCenter)
{
    //    std::vector<cv::Point2f> blobCenter;
    for(int i = 0; i < blobs.size(); i++)
    {
        float xCenter = 0;
        float yCenter = 0;
        for(int j=0; j < blobs[i].size(); j++)
        {
            int x = blobs[i][j].x;
            int y = blobs[i][j].y;
            xCenter += (float)x/(float)blobs[i].size();
            yCenter += (float)y/(float)blobs[i].size();
        }
        blobCenter.push_back(cv::Point2f(xCenter,yCenter));
        //        qDebug() << i << blobs[i].size() << blobCenter[i].x << blobCenter[i].y << this->calcualteCOV(blobs[i]);
    }
}

void tag_recognition::cutWords(cv::Mat wordsMask, cv::Mat rawDst, cv::Mat &word1, cv::Mat &word2)
{
    std::vector < std::vector<cv::Point2f> > rotatedBlobs;
    cv::normalize(wordsMask,wordsMask,0,1,cv::NORM_MINMAX);
    this->findBlobs(wordsMask,rotatedBlobs);
    rotatedBlobs = this->removeImpossibleBlobs(rotatedBlobs);
    //    qDebug() << rotatedBlobs.size();
    if(rotatedBlobs.size() < 2)
    {
        cv::normalize(wordsMask,wordsMask,0,255,cv::NORM_MINMAX);
        cv::imshow("WTF",wordsMask);
        word1 = cv::Mat::zeros(1,1,CV_8UC1);
        word2 = cv::Mat::zeros(1,1,CV_8UC1);
        return;
    }

    for(int i = 0; i < rotatedBlobs.size(); i++)
    {
        cv::Point2f topLeft = cv::Point2f(tagSize,tagSize);
        cv::Point2f downRight = cv::Point2f(0,0);
        for(int j=0; j < rotatedBlobs[i].size(); j++)
        {
            if(topLeft.x > rotatedBlobs[i][j].x)
                topLeft.x = rotatedBlobs[i][j].x;
            if(topLeft.y > rotatedBlobs[i][j].y)
                topLeft.y = rotatedBlobs[i][j].y;
            if(downRight.x < rotatedBlobs[i][j].x)
                downRight.x = rotatedBlobs[i][j].x;
            if(downRight.y < rotatedBlobs[i][j].y)
                downRight.y = rotatedBlobs[i][j].y;
        }
        //        qDebug() << topLeft.x << topLeft.y << downRight.x << downRight.y;
        if(i == 0)
        {
            cv::getRectSubPix(rawDst,cv::Size(downRight.x-topLeft.x+4,downRight.y-topLeft.y+4),(downRight+topLeft)/2,word1);
        }
        else if(i == 1)
        {
            cv::getRectSubPix(rawDst,cv::Size(downRight.x-topLeft.x+4,downRight.y-topLeft.y+4),(downRight+topLeft)/2,word2);
        }


        //        cv::imshow(std::to_string(i),dst);
    }
}

