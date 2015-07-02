#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stitcher = new cam_input;
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
    QStringList fileNames = QFileDialog::getOpenFileNames();
    videoNames.clear();
    videoNames.resize(fileNames.size());
    for(int i = 0; i < fileNames.size(); i++)
    {
        videoNames[i] = fileNames.at(i).toStdString();
    }
    stitcher->setVideoName(videoNames);
    stitcher->start();

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
