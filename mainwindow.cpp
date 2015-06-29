#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    cv::VideoCapture video[3];
    for (int i=0;i<3;i++)
    {
        video[i].open(fileNames[i].toStdString());
    }
    cv::Mat frame[3];
    while(1)
    {
        video[0].read(frame[0]);
        cv::imshow("video",frame[0]);

        if(cv::waitKey(1000.0/fps)==27)
        {
            cv::destroyAllWindows();
            break;
        }
    }
}


void MainWindow::on_actionLoad_Stitching_Image_triggered()
{

}
