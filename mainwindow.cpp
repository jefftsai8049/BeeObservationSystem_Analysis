#include "mainwindow.h"
#include "ui_mainwindow.h"

std::vector<cv::Mat> stitchFrame;

std::vector<cv::Point> originPoint;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stitcher = new cam_input;
    TT = new trajectory_tracking;

    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(stitcher,SIGNAL(sendPano(cv::Mat)),this,SLOT(receivePano(cv::Mat)));
    connect(stitcher,SIGNAL(stitchFinish()),this,SLOT(stitchImage()));
#ifdef QT_DEBUG
    qDebug() << "Running a debug build";
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionLoad_Raw_Video_File_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory();

    dir.setPath(dirName);
    QStringList dirList = dir.entryList(QDir::Dirs,QDir::Name);

    QStringList nameFilter;
    nameFilter.append("*.avi");


    for (int j = 0;j<dirList.size()-2;j++)
    {
        dir.cd(dirList[j+2]);
        videoList.push_back(dir.entryList(nameFilter,QDir::Files,QDir::Name));
        dir.cdUp();
    }
#ifdef QT_DEBUG
    qDebug () << videoList[0] << videoList[1] << videoList[2];
#endif
    for (int k = 0;k<videoList[0].size();k++)
    {
        QString fileName = videoList[0][k]+"\n";
        fileName = fileName.mid(2,fileName.length()-2);
        ui->videoName_textBrowser->insertPlainText(fileName);
    }


}


void MainWindow::on_actionLoad_Stitching_Image_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames();


    std::vector<std::string> fileNamesVec;
    for(int i = 0; i < fileNames.size(); i++)
    {
        fileNamesVec.push_back(fileNames[i].toStdString());
    }
    stitcher->initialVideo(fileNamesVec);

}

void MainWindow::receivePano(cv::Mat pano)
{
    cv::Mat show;
    cv::resize(pano,show,cv::Size(pano.cols/2,pano.rows/2));
    cv::imshow("Panorama",show);
}

void MainWindow::stitchImage()
{

    qDebug() << "Stitching Image";

    std::vector<std::string> fileNames;
    std::string path = dir.absolutePath().toStdString();
    fileNames = getVideoName(videoList,path);
    for(int i = 0;i<videoList.size();i++)
    {
        videoList[i].erase(videoList[i].begin());
    }

    if (stitchMode == 1)
    {
        stitcher->setVideoName(fileNames);
        stitcher->start();
    }
    else if(stitchMode == 0)
    {
        if(manualLoad)
        {
            TT->imageShift(stitchFrame);
        }
    }
}

void MainWindow::changeStitchMode()
{
    stitchMode = 1;//auto
}

std::vector<std::string> MainWindow::getVideoName(QVector<QStringList> list,std::string path)
{
    std::vector<std::string> fileNames;
    for(int i = 0;i<videoList.size();i++)
    {
        if (list[i].size()<1)
        {
            //            qDebug() << "END";
            return fileNames;
        }
        std::string folder;
        if (i == 0)
            folder = "/Camera_L/";
        else if(i == 1)
            folder = "/Camera_M/";
        else if(i == 2)
            folder = "/Camera_R/";
        //        qDebug() << QString::fromStdString(path+folder+list[i][0].toStdString());
        fileNames.push_back(path+folder+list[i][0].toStdString());
    }
    return fileNames;
}

void mouseCallBack(int event, int x, int y, int flag,void* userdata)
{
    cv::Size imageSize = cv::Size(imgSizeX,imgSizeY);



    static cv::Point lastPoint;
    cv::Point shiftDelta = cv::Point(0,0);
    int imgIndex;

    if (event == 1)
    {
        lastPoint = cv::Point(x,y);
        return;
    }
    else if (event == 3)
    {
        qDebug() << "fileSaved";
        cv::FileStorage f("manual_stitching.xml",cv::FileStorage::WRITE);
        f << "point" << originPoint;
        f.release();

    }
    if (flag == 1)
    {
        //        qDebug() << "Last" << lastPoint.x << lastPoint.y;
        qDebug() << event << x << y << flag;
        shiftDelta = cv::Point(x,y)-lastPoint;
        if (x > originPoint[0].x && x <= originPoint[0].x+imgSizeX)
        {
            imgIndex = 0;
        }
        if (x > originPoint[1].x && x <= originPoint[1].x+imgSizeX)
        {
            imgIndex = 1;
        }
        if (x > originPoint[2].x && x <= originPoint[2].x+imgSizeX)
        {
            imgIndex = 2;
        }

        originPoint[imgIndex].x = originPoint[imgIndex].x+shiftDelta.x;
        originPoint[imgIndex].y = originPoint[imgIndex].y+shiftDelta.y;
        //        qDebug() << imgIndex << shiftDelta.x << shiftDelta.y;
        //        qDebug() << originPoint[imgIndex].x <<originPoint[imgIndex].y;

        lastPoint = cv::Point(x,y);

        qDebug() << originPoint[0].x << originPoint[0].y << originPoint[1].x << originPoint[1].y << originPoint[2].x << originPoint[2].y;
        cv::Mat cat(cv::Size(imgSizeX*3,imgSizeY),CV_8UC3,cv::Scalar(0));
        for (int i=0;i<3;i++)
        {
            if(originPoint[i].x>=0&&originPoint[i].y>=0)
            {
                stitchFrame[i](cv::Rect(0,0,fmin(imgSizeX,imgSizeX*3-originPoint[i].x),imgSizeY-originPoint[i].y)).copyTo(cat(cv::Rect(originPoint[i].x,originPoint[i].y,fmin(imgSizeX,imgSizeX*3-originPoint[i].x),imgSizeY-originPoint[i].y)));
            }

        }

        cv::imshow("Stitch",cat);
//        trajectory_tracking t;
//        t.imageShift(stitchFrame,originPoint);
//        qDebug() <<"fuck3";

    }

}

void MainWindow::on_stitchingStart_pushButton_clicked()
{
    if (stitchMode == 0)
    {

        std::vector<std::string> fileNames;
        std::string path = dir.absolutePath().toStdString();
        fileNames = getVideoName(videoList,path);
        for(int i = 0;i<videoList.size();i++)
        {
            cv::VideoCapture cap(fileNames[i]);
            cv::Mat temp;
            cap.read(temp);
            stitchFrame.push_back(temp);
            cap.release();
        }

        if (!manualLoad)
        {
            originPoint.resize(3);
            originPoint[0] = cv::Point(0,0);
            originPoint[1] = cv::Point(imgSizeX,0);
            originPoint[2] = cv::Point(imgSizeX*2,0);

            cv::Mat cat;
            cv::hconcat(stitchFrame,cat);
            cv::namedWindow("Stitch");
            cv::setMouseCallback("Stitch",mouseCallBack,0);
            cv::imshow("Stitch",cat);
            manualLoad = 1;
        }
        else
        {
            stitchImage();

        }


    }
    else if (stitchMode == 1)
    {
        std::vector<std::string> fileNames;
        std::string path = dir.absolutePath().toStdString();
        fileNames = getVideoName(videoList,path);
        for(int i = 0;i<videoList.size();i++)
        {
            cv::VideoCapture cap(fileNames[i]);
            cv::Mat temp;
            cap.read(temp);
            stitchFrame.push_back(temp);
            cap.release();
        }
        stitchImage();

    }




}

void MainWindow::on_stitchingStop_pushButton_clicked()
{
    stitcher->stopStitch(true);
}

void MainWindow::on_actionLoad_Maunal_Stitching_Setting_triggered()
{
    cv::FileStorage f("manual_stitching.xml",cv::FileStorage::READ);
    std::vector<cv::Point> p;
    if (f.isOpened())
    {

        f["point"] >> p;
        f.release();
    }
    TT->setImageShiftOriginPoint(p);
    manualLoad = 1;
}
