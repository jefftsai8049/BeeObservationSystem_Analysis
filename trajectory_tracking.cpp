#include "trajectory_tracking.h"

trajectory_tracking::trajectory_tracking(QObject *parent) : QThread(parent)
{
    frame.resize(3);
    //    circleDetect = new cv::cuda::HoughCirclesDetector;
    TR = new tag_recognition;


    cv::ocl::setUseOpenCL(true);


    qDebug() << "ocl" << cv::ocl::useOpenCL() << cv::ocl::haveOpenCL();

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

void trajectory_tracking::setVideoName(std::vector<std::string> videoName)
{
    this->videoName = videoName;

}

void trajectory_tracking::setCircleDectionMode(const int &mode)
{
    //mode 0 Hough Transform
    //mode 1 Contour

    this->circleDectionMode = mode;
}

void trajectory_tracking::setHoughCircleParameters(const int &dp,const int &minDist,const int &para_1,const int &para_2,const int &minRadius,const int &maxRadius)
{
    qDebug() << "set";
    this->dp = dp;

    this->minDist = minDist;

    this->para_1 = para_1;

    this->para_2 = para_2;

    this->minRadius = minRadius;

    this->maxRadius = maxRadius;
    //cuda testing
    //    circleDetect->setDp(this->dp);
    //    circleDetect->setMinDist(this->minDist);
    //    circleDetect->setCannyThreshold(this->para_1);
    //    circleDetect->setVotesThreshold(this->para_2);

    //    circleDetect->setMinRadius(this->minRadius);
    //    circleDetect->setMaxRadius(this->maxRadius);
}

void trajectory_tracking::setContourParamters(const int &threshold,const int &area)
{
    this->contourThreshold = threshold;
    this->contourArea = area;
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

    //load SVM model
    if(!TR->loadSVMModel("svm_grid_search_opt.yaml"))
    {
        return;
    }

    //cuda testing
    //    qDebug() << cv::cuda::getCudaEnabledDeviceCount();
    //    qDebug() << cv::cuda::getDevice();

    //    cv::Ptr<cv::cuda::HoughCirclesDetector> circleDetect = cv::cuda::createHoughCirclesDetector(this->dp,this->minDist,this->para_1,this->para_2,this->minRadius,this->maxRadius);
    //    cv::cuda::DeviceInfo gpu;
    //    qDebug() << gpu.name();
    //    qDebug() << cv::cuda::getCudaEnabledDeviceCount();
    //    qDebug() << cv::cuda::getDevice();
    //    cv::Ptr<cv::cuda::HoughCirclesDetector> circleDetect = cv::cuda::createHoughCirclesDetector(this->dp,this->minDist,this->para_1,this->para_2,this->minRadius,this->maxRadius);
    //    qDebug() << "cuda ok";

    //thread stop flag
    this->stopped = false;


    //    cv::cuda::GpuMat panoGpu;
    //    cv::cuda::GpuMat circlesGpu;

    int frameCount = 0;


    //main processing loop
    while(!this->stopped)
    {
        //calculate fps
        QTime clock;
        clock.start();
        std::vector<cv::Mat> frame(3);
        std::vector<cv::Mat> frameGray(3);


        //capture frame and convert to gray


        for(int i = 0; i < 3; i++)
        {
            cap[i] >> frame[i];
            cv::cvtColor(frame[i],frameGray[i],cv::COLOR_BGR2GRAY);
//            frameGray[i] = this->bgr2gray(frame[i]);
//            qDebug() << m.elapsed();
        }
//        qDebug() << "m.elapsed()";
        if(frame[0].empty()||frame[1].empty()||frame[2].empty())
        {
            this->stopped = true;
            break;
        }

        cv::Mat pano;
        cv::UMat panoUMAT;
        //stitching image
        pano = this->imageShiftLoaded(frameGray);
        pano = this->imageCutBlack(pano);

//        pano = panoUMAT.getMat(2);

        panoUMAT = pano.getUMat(cv::ACCESS_READ);
        //mode 0 Hough Transform
        //mode 1 Contour
//        cv::UMat panoUMAT = pano.clone().getUMat(cv::ACCESS_READ);
        if(this->circleDectionMode == 0)
        {
            //hough circle detection
            std::vector<cv::Vec3f> circles;

            cv::HoughCircles(panoUMAT,circles,CV_HOUGH_GRADIENT,dp,minDist,para_1,para_2,minRadius,maxRadius);

            std::vector<cv::Mat> circleImg(circles.size());
            std::vector<std::string> w1,w2;
            for (int i=0;i<circles.size();i++)
            {
                cv::getRectSubPix(pano,cv::Size(circles[i][2]*2-1,circles[i][2]*2-1),cv::Point(circles[i][0], circles[i][1]),circleImg[i]);
                cv::Mat word1,word2;
                TR->tagImgProc(circleImg[i],word1,word2);

                w1.resize(circles.size());
                w2.resize(circles.size());
                w1[i].push_back(TR->wordRecognition(word1));
                w2[i].push_back(TR->wordRecognition(word2));
//                qDebug() << QString::fromStdString(w1[i])<<QString::fromStdString(w2[i]);


            }

            if (showImage)
            {


                //draw cicle
                //                qDebug() << circles.size();
                cv::Mat panoDrawCircle = pano.clone();
                for(int i = 0; i < circles.size(); i++ )
                {
                    cv::Point center(circles[i][0], circles[i][1]);
                    int radius = circles[i][2];
                    cv::circle( panoDrawCircle, center, radius, cv::Scalar(255), 1, 8, 0 );
                    cv::putText(panoDrawCircle,w1[i],cv::Point(circles[i][0]-15, circles[i][1]+40),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(255));
                    cv::putText(panoDrawCircle,w2[i],cv::Point(circles[i][0]+5, circles[i][1]+40),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(255));
                }


                //                cv::waitKey(3);
                //cut subimage

                //                for (int i=0;i<circles.size();i++)
                //                {


                //                    cv::imshow("word1",word1);
                //                    cv::imshow("word2",word2);
                //                char w1,w2;

                //                    cv::Mat wordReshpae1 = word1.clone();
                //                    cv::Mat wordReshpae2 = word2.clone();
                //                    cv::normalize(word1,word1,0,255,cv::NORM_MINMAX);
                //                    cv::normalize(word2,word2,0,255,cv::NORM_MINMAX);
                //                    cv::imwrite("SVM/"+w1+"/"+std::to_string(frameCount)+"_"+std::to_string(i)+"1.jpg",word1);
                //                    cv::imwrite("SVM/"+w2+"/"+std::to_string(frameCount)+"_"+std::to_string(i)+"2.jpg",word2);
                //                    qDebug() << QString::fromStdString(w1) << QString::fromStdString(w2);


                cv::resize(panoDrawCircle,panoDrawCircle,cv::Size(panoDrawCircle.cols/2,panoDrawCircle.rows/2));
                //show image
                cv::imshow("Stitch",panoDrawCircle);
                //                cv::imwrite("word/"+std::to_string(frameCount)+"_"+std::to_string(i)+"_1.jpg",word1);
                //                cv::imwrite("word/"+std::to_string(frameCount)+"_"+std::to_string(i)+"_2.jpg",word2);
                //                cv::imshow("tag",circleImg[i]);
                //                cv::waitKey(100);
                //            cv::imwrite("tag/"+std::to_string(frameCount)+"_"+std::to_string(i)+".jpg",circleImg[i]);
                //                }
//                cv::waitKey(50);



            }
            else if (this->circleDectionMode == 1)
            {
                cv::Mat canny_output;
                std::vector<std::vector<cv::Point> > contours;
                std::vector<cv::Vec4i> hierarchy;


                //Detect edges using canny
                cv::Canny( pano, canny_output, this->contourThreshold, this->contourThreshold*2, 3 );
                //Find contours
                cv::findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );


                if (showImage)
                {
                    //Draw contours
                    //                cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC1 );
                    //                for( int i = 0; i< contours.size(); i++ )
                    //                {
                    //                    cv::Scalar color = cv::Scalar(255);
                    //                    cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
                    //                }
                    //                cv::resize(drawing,drawing,cv::Size(drawing.cols/2,drawing.rows/2));
                    //                cv::imshow("contour",drawing);
                    for(int i = 0;i < contours.size();i++)
                    {
                        if(cv::contourArea(contours[i]) > this->contourArea)
                        {
                            double x = 0;
                            double y = 0;
                            for (int j = 0;j < contours[i].size();j++)
                            {
                                x += contours[i][j].x/contours[i].size();
                                y += contours[i][j].y/contours[i].size();
                            }
                            cv::circle(pano,cv::Point(x,y),15,cv::Scalar(255));
                        }
                    }


                    cv::resize(pano,pano,cv::Size(pano.cols/2,pano.rows/2));
                    cv::imshow("Stitch",pano);
                    //                cv::waitKey(3);
                }

            }





        }


        //cuda testing
        //        panoGpu.upload(pano);
        //        circleDetect->detect(panoGpu,circlesGpu);

        //        cv::Mat circlesMat;
        //        circlesGpu.download(circlesMat);

        //        std::vector<cv::Vec3f> circles(circlesMat.cols);
        //        qDebug() << circles.cols <<circles.rows << circles.channels();
        //        if (circlesMat.cols>0)
        //        {
        //            for (int i=0;i<circlesMat.cols;i++)
        //            {
        //               qDebug() << "x" << circlesMat.data[i];
        //               qDebug() << "y" << circlesMat.data[i+circlesMat.cols];
        //               qDebug() << "r" << circlesMat.data[i+circlesMat.cols*2];
        //               circles[i] =  cv::Vec3f(circlesMat.data[i],circlesMat.data[i+circlesMat.cols],circlesMat.data[i+circlesMat.cols*2]);
        //            }
        //        }
        frameCount++;
        emit sendFPS(1000.0/clock.elapsed());
    }



    for(int i = 0; i < 3; i++)
    {
        cap[i].release();
    }
    qDebug() << "finish";
    emit finish();
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

cv::Mat trajectory_tracking::imageCutBlack(cv::Mat src)
{
    cv::Mat dst(src, cv::Rect(0, 0, originPoint[2].x+imgSizeX, imgSizeY));
    return dst;
}

