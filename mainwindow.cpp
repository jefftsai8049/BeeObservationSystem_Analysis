#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stitcher = new cam_input;

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
    else
    {

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

void MainWindow::on_stitchingStart_pushButton_clicked()
{
    if (stitchMode == 0)
    {
        std::vector<cv::Mat> frame;
        std::vector<std::string> fileNames;
        std::string path = dir.absolutePath().toStdString();
        fileNames = getVideoName(videoList,path);
        for(int i = 0;i<videoList.size();i++)
        {
            cv::VideoCapture cap(fileNames[i]);
            cv::Mat temp;
            cap.read(temp);
            frame.push_back(temp);
            cap.release();
        }
        cv::Mat cat;
        cv::hconcat(frame,cat);
        cv::imshow("test",cat);
    }
    stitchImage();
}

void MainWindow::on_stitchingStop_pushButton_clicked()
{
    stitcher->stopStitch(true);
}
