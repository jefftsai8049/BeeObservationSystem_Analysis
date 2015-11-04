#include "trajectory_tracking.h"

trajectory_tracking::trajectory_tracking(QObject *parent) : QThread(parent)
{
    frame.resize(3);

    TR = new tag_recognition;



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

cv::Mat trajectory_tracking::imageShiftLoaded(std::vector<cv::Mat> stitchFrame)
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

void trajectory_tracking::initOCL()
{
    cv::ocl::setUseOpenCL(true);

    if(cv::ocl::useOpenCL())
    {
        emit sendSystemLog("OpenCL is enabled");
    }
}

void trajectory_tracking::setVideoName(std::vector<std::string> videoName)
{
    this->videoName = videoName;

}


void trajectory_tracking::setHoughCircleParameters(const int &dp,const int &minDist,const int &para_1,const int &para_2,const int &minRadius,const int &maxRadius)
{
    emit sendSystemLog("Hough circles parametrs set");

    this->dp = dp;

    this->minDist = minDist;

    this->para_1 = para_1;

    this->para_2 = para_2;

    this->minRadius = minRadius;

    this->maxRadius = maxRadius;
}

void trajectory_tracking::setShowImage(const bool &status)
{
    showImage = status;
}

void trajectory_tracking::setSVMModelFileName(const std::string &fileName)
{
    this->SVMModelFileName = fileName;
}

void trajectory_tracking::setPCAModelFileName(const std::string &fileName)
{
    this->PCAModelFileName = fileName;
}

void trajectory_tracking::setManualStitchingFileName(const std::string &fileName)
{
    cv::FileStorage f(fileName,cv::FileStorage::READ);
    std::vector<cv::Point> p;
    if (f.isOpened())
    {

        f["point"] >> p;
        f.release();
    }
    this->setImageShiftOriginPoint(p);
}

void trajectory_tracking::setTagBinaryThreshold(const double &value)
{
    TR->setTagBinaryThreshold(value);
}

void trajectory_tracking::setPCAandHOG(const bool &PCAS, const bool &HOGS)
{
    TR->setPCAandHOG(PCAS,HOGS);
}

void trajectory_tracking::stopStitch()
{
    this->stopped = true;
}

void trajectory_tracking::run()
{
    emit sendSystemLog("Processing start!\n"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm")+"\n");

    //open video file
    std::vector<cv::VideoCapture> cap(3);
    std::vector<float> deviceFPS(3);
    std::vector<float> nowFrameCount(3);

    std::stringstream msgSS;
    msgSS << "Processing files :\n";

    for(int i = 0; i < 3; i++)
    {
        cap[i].open(this->videoName[i]);
        if(!cap[i].isOpened())
        {
            return;
        }
        deviceFPS[i] = (float)cap[i].get(CV_CAP_PROP_FRAME_COUNT);
        nowFrameCount[i] = 0;

        msgSS << videoName[i] << " FPS :" << deviceFPS[i]/VIDEOTIME << "\n";
    }


    emit sendSystemLog(QString::fromStdString(msgSS.str()));

    float maxFPS = mf::findMax(deviceFPS[0],deviceFPS[1],deviceFPS[2]);
    for(int j = 0;j<deviceFPS.size();j++)
    {
        deviceFPS[j] = deviceFPS[j]/maxFPS;
    }


    //load SVM model
    if(!TR->loadSVMModel(SVMModelFileName))
    {
        return;
    }
    if(!TR->loadPCAModel(PCAModelFileName))
    {
        return;
    }

    //thread stop flag
    this->stopped = false;
    int frameCount = 0;

    std::vector<cv::Mat> frame(3);
    std::vector<cv::Mat> frameGray(3);



    //main processing loop
    while(!this->stopped)
    {

        //calculate fps
        QTime clock;
        clock.start();



        //capture frame and convert to gray
        for(int i = 0; i < 3; i++)
        {
            //for fit different FPS
            if(frameCount == 0)
            {
                cap[i] >> frame[i];
            }
            else
            {
                nowFrameCount[i] += deviceFPS[i];
                if(nowFrameCount[i] > 1)
                {
                    cap[i] >> frame[i];
                    nowFrameCount[i]--;
                }
            }
            //convert to gray
            if(frame[i].channels() ==3)
            {
                cv::cvtColor(frame[i],frameGray[i],cv::COLOR_BGR2GRAY);
            }
        }
        //end of file?
        if(frame[0].empty()||frame[1].empty()||frame[2].empty())
        {
            this->stopped = true;
            break;
        }

        cv::Mat pano;
        //stitching image
        pano = this->imageShiftLoaded(frameGray);
        pano = this->imageCutBlack(pano);

        //hough circle detection
        std::vector<cv::Vec3f> circles;
        cv::Mat panoSmall;
        cv::resize(pano,panoSmall,cv::Size(pano.cols/2,pano.rows/2));
        cv::HoughCircles(panoSmall,circles,CV_HOUGH_GRADIENT,dp,minDist,para_1,para_2,minRadius,maxRadius);

        std::vector<cv::Mat> circleImg;
        circleImg.resize(circles.size());

#ifndef DEBUG_TAG_RECOGNITION
        std::vector<std::string> w1;
        std::vector<std::string> w2;
        w1.resize(circles.size());
        w2.resize(circles.size());
#endif

#pragma omp parallel for
        for (int j=0;j<circles.size();j++)
        {
            cv::Mat tagImg;
            cv::getRectSubPix(pano,cv::Size(circles[j][2]*2*2+11,circles[j][2]*2*2+11),cv::Point(circles[j][0]*2, circles[j][1]*2),tagImg);
            cv::Mat word1,word2;
            TR->tagImgProc(tagImg,word1,word2);


#ifndef DEBUG_TAG_RECOGNITION
            w1[j].push_back(TR->wordRecognition(word1));
            w2[j].push_back(TR->wordRecognition(word2));
            //            cv::imshow("w1",word1);
            //            cv::imshow("w2",word2);

            //            cv::normalize(word1,word1,0,255,cv::NORM_MINMAX);
            //            cv::normalize(word2,word2,0,255,cv::NORM_MINMAX);
            //            cv::imwrite("SVM/"+w1[j]+"/"+std::to_string(frameCount)+"_"+std::to_string(i)+"1.jpg",word1);
            //            cv::imwrite("SVM/"+w2[j]+"/"+std::to_string(frameCount)+"_"+std::to_string(i)+"2.jpg",word2);
            //            qDebug() << QString::fromStdString(w1[j]) << QString::fromStdString(w2[j]);
#endif
        }


        if (showImage)
        {
            //draw cicle
            cv::Mat panoDrawCircle;
            cv::cvtColor(pano,panoDrawCircle,cv::COLOR_GRAY2BGR);

            for(int i = 0; i < circles.size(); i++ )
            {
                cv::Point center(circles[i][0]*2, circles[i][1]*2);
                int radius = circles[i][2]*2;
                cv::circle( panoDrawCircle, center, radius, cv::Scalar(0,0,255), 1, 8, 0 );
#ifndef DEBUG_TAG_RECOGNITION
                cv::putText(panoDrawCircle,w1[i],cv::Point(circles[i][0]*2-15, circles[i][1]*2+40),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(0,0,255));
                cv::putText(panoDrawCircle,w2[i],cv::Point(circles[i][0]*2+5, circles[i][1]*2+40),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(0,0,255));
#endif
            }

            cv::resize(panoDrawCircle,panoDrawCircle,cv::Size(panoDrawCircle.cols/2,panoDrawCircle.rows/2));
            emit sendImage(panoDrawCircle);

        }


        frameCount++;
        emit sendFPS(1000.0/clock.elapsed());

        //        cv::waitKey(1000);
    }


    //close video file and emit finish signal
    for(int i = 0; i < 3; i++)
    {
        cap[i].release();
    }
    emit sendSystemLog("Processing finish!\n"+QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm")+"\n");
    emit finish();


}

cv::Mat trajectory_tracking::imageCutBlack(cv::Mat src)
{

    cv::Mat dst(src, cv::Rect(0, 0, originPoint[2].x+imgSizeX, imgSizeY));
    return dst;
}

