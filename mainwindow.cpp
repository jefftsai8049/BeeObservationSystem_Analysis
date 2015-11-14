#include "mainwindow.h"
#include "ui_mainwindow.h"

std::vector<cv::Mat> stitchFrame;

std::vector<cv::Point> originPoint;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //load icon image
    this->setWindowIcon(QIcon("icon/honeybee.jpg"));
    //for tracking honeybee position
    TT = new trajectory_tracking;

    qRegisterMetaType<cv::Mat>("cv::Mat");
    //send execute speed back to mainwindow
    connect(TT,SIGNAL(sendFPS(double)),this,SLOT(receiveFPS(double)));
    //send image back to mainwindow
    connect(TT,SIGNAL(sendImage(cv::Mat)),this,SLOT(receiveShowImage(cv::Mat)));

    connect(TT,SIGNAL(sendSystemLog(QString)),this,SLOT(receiveSystemLog(QString)));

    connect(this,SIGNAL(sendSystemLog(QString)),this,SLOT(receiveSystemLog(QString)));

    //trajectory tracking parameters setting
    TT->setTagBinaryThreshold(ui->binarythreshold_spinBox->value());
    TT->setManualStitchingFileName("manual_stitching.xml");
    TT->setSVMModelFileName("model/model_HOG_PCA_25_1_0.984706.yaml");
    TT->setPCAModelFileName("model/PCA_HOG_PCA_25_.txt");
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
    TT->setPCAandHOG(ui->actionWith_PCA->isChecked(),ui->actionWith_HOG->isChecked());

    TT->initOCL();

#ifdef DEBUG_TAG_RECOGNITION
    emit sendSystemLog("Tag Recognition Debuging");
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionLoad_Raw_Video_File_triggered()
{

    QString dirName = QFileDialog::getExistingDirectory();

    if(dirName.isEmpty())
    {
        return;
    }

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

    if(fileNames.isEmpty())
    {
        return;
    }

    std::vector<std::string> fileNamesVec;
    for(int i = 0; i < fileNames.size(); i++)
    {
        fileNamesVec.push_back(fileNames[i].toStdString());
    }
}

void MainWindow::receivePano(cv::Mat pano)
{
    cv::Mat show;
    cv::resize(pano,show,cv::Size(pano.cols/2,pano.rows/2));
    cv::imshow("Panorama",show);
}

void MainWindow::receiveFPS(const double &fpsRun)
{
    ui->processing_lcdNumber->display(fpsRun);
}

void MainWindow::receiveShowImage(const cv::Mat &src)
{

    static bool status = 0;
    if(status == 0)
    {
        ui->imageShow_widget->initialize(1,src.cols,src.rows);
        status = 1;
    }
    ui->imageShow_widget->setImage(src);

}

void MainWindow::receiveSystemLog(const QString &msg)
{
    ui->system_log_textBrowser->append(msg);
}

void MainWindow::stitchImage()
{
    qDebug() << "start";
    std::vector<std::string> fileNames;
    std::string path = dir.absolutePath().toStdString();
    fileNames = getVideoName(videoList,path);
    for(int i = 0;i<videoList.size();i++)
    {
        videoList[i].erase(videoList[i].begin());
    }


    TT->setVideoName(fileNames);
    TT->start();

    ui->statusBar->showMessage(QString::fromStdString(fileNames[1])+" is processing...");

}


std::vector<std::string> MainWindow::getVideoName(QVector<QStringList> list,std::string path)
{
    std::vector<std::string> fileNames;
    for(int i = 0;i<videoList.size();i++)
    {
        if (list[i].size()<1)
        {
            return fileNames;
        }
        std::string folder;
        if (i == 0)
            folder = "/Camera_L/";
        else if(i == 1)
            folder = "/Camera_M/";
        else if(i == 2)
            folder = "/Camera_R/";
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


    }

}

void MainWindow::on_stitchingStart_pushButton_clicked()
{
    connect(TT,SIGNAL(finish()),this,SLOT(on_stitchingStart_pushButton_clicked()));
    stitchImage();
}

void MainWindow::on_stitchingStop_pushButton_clicked()
{
    disconnect(TT,SIGNAL(finish()),this,SLOT(on_stitchingStart_pushButton_clicked()));

    TT->stopStitch();

}

void MainWindow::on_stitching_pushButton_clicked()
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

        originPoint.resize(3);
        originPoint[0] = cv::Point(0,0);
        originPoint[1] = cv::Point(imgSizeX,0);
        originPoint[2] = cv::Point(imgSizeX*2,0);

        cv::Mat cat;
        cv::hconcat(stitchFrame,cat);
        cv::namedWindow("Stitch");
        cv::setMouseCallback("Stitch",mouseCallBack,0);
        cv::imshow("Stitch",cat);
    }
    else
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

    }
}

void MainWindow::on_dp_hough_circle_spinBox_valueChanged(int arg1)
{
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
}

void MainWindow::on_minDist_hough_circle_spinBox_valueChanged(int arg1)
{
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
}

void MainWindow::on_para_1_hough_circle_spinBox_valueChanged(int arg1)
{
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
}

void MainWindow::on_para_2_hough_circle_spinBox_valueChanged(int arg1)
{
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
}

void MainWindow::on_minRadius_hough_circle_spinBox_valueChanged(int arg1)
{
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
}

void MainWindow::on_maxRadius_hough_circle_spinBox_valueChanged(int arg1)
{
    TT->setHoughCircleParameters(ui->dp_hough_circle_spinBox->value(),ui->minDist_hough_circle_spinBox->value(),ui->para_1_hough_circle_spinBox->value(),ui->para_2_hough_circle_spinBox->value(),ui->minRadius_hough_circle_spinBox->value(),ui->maxRadius_hough_circle_spinBox->value());
}

void MainWindow::on_show_image_checkBox_clicked()
{
    TT->setShowImage(ui->show_image_checkBox->isChecked());
}

void MainWindow::on_load_training_data_pushButton_clicked()
{


}

void MainWindow::on_test_recognition_pushButton_clicked()
{

}


void MainWindow::on_binarythreshold_spinBox_valueChanged(int arg1)
{
    TT->setTagBinaryThreshold(arg1);
}

void MainWindow::on_actionChange_SVM_Model_triggered()
{
    QString fileNameQ = QFileDialog::getOpenFileName(this,"SVM Model (.yaml)","","Model Files (*.yaml)");
    TT->setSVMModelFileName(fileNameQ.toStdString());
    ui->statusBar->showMessage("Change SVM model "+fileNameQ);
}

void MainWindow::on_actionChange_PCA_Model_triggered()
{
    QString fileNameQ = QFileDialog::getOpenFileName(this,"PCA Model (.txt)","","Model Files (*.txt)");
    TT->setPCAModelFileName(fileNameQ.toStdString());
    ui->statusBar->showMessage("Change PCA model "+fileNameQ);
}

void MainWindow::on_actionChange_Stitching_Model_triggered()
{
    QString fileNameQ = QFileDialog::getOpenFileName(this,"Stitching Model (.xml)","","Model Files (*.xml)");
    TT->setManualStitchingFileName(fileNameQ.toStdString());
    ui->statusBar->showMessage("Change Stitching model "+fileNameQ);
}

void MainWindow::on_actionTrain_New_Tag_Model_triggered()
{
    //load train data
    QString fileName = QFileDialog::getExistingDirectory();
    cv::Mat trainData;
    cv::Mat trainLabel;

    TR->loadTrainData(fileName,trainData,trainLabel,ui->actionWith_HOG->isChecked());
    qDebug() << trainData.channels() << trainData.cols << trainData.rows;
    //load test data
    QString fileName2 = QFileDialog::getExistingDirectory();
    std::vector<cv::Mat> testData;
    std::vector<int> testLabel;
    TR->loadTestData(fileName2,testData,testLabel,ui->actionWith_HOG->isChecked());



    //train PCA model and save
    if(ui->actionWith_PCA->isChecked())
    {
        pca = new cv::PCA(trainData,cv::Mat(),cv::PCA::DATA_AS_ROW,ui->PCARemains_spinBox->value());
        trainData = pca->project(trainData);
        cv::normalize(trainData,trainData,0,1,cv::NORM_MINMAX);


        QString name = "model/PCA_";
        if(ui->actionWith_HOG->isChecked())
        {
            name = name+"HOG_";
        }
        if(ui->actionWith_PCA->isChecked())
        {
            name = name+"PCA_"+QString::number(ui->PCARemains_spinBox->value())+"_";
        }
        cv::FileStorage PCA_save((name+".txt").toStdString(),cv::FileStorage::WRITE);



        pca->write(PCA_save);
    }

    cv::Ptr<cv::ml::SVM> SVMModel = cv::ml::SVM::create();
    SVMModel->setType(cv::ml::SVM::C_SVC);
    SVMModel->setKernel(cv::ml::SVM::LINEAR);
    int C_lower = -5;
    int C_upper = 15;


    //calculate accuracy
    for(int i=0;i<testData.size();i++)
    {
        if(ui->actionWith_PCA->isChecked())
        {
            testData[i] = pca->project(testData[i]);
        }
        cv::normalize(testData[i],testData[i],0,1,cv::NORM_MINMAX);
    }

    for(int j=0;j<(C_upper-C_lower);j++)
    {
        //        for(int k=0;k<(Gamma_upper-Gamma_lower);k++)
        //        {
        //train SVM model
        qDebug() << "SVM";

        //            int k =3;
        //            int j = 14;
        //            SVMModel->setGamma(pow(2,k+Gamma_lower));
        SVMModel->setC(pow(2,j+C_lower));
        //                                    SVMModel->setGamma(pow(2,-12));
        //                                    SVMModel->setC(pow(2,9));
        //            qDebug() << "Set" << j+C_lower << k+Gamma_lower;
        qDebug() << "Set" << j+C_lower;
        cv::Ptr<cv::ml::TrainData> data;
        data = cv::ml::TrainData::create(trainData,cv::ml::ROW_SAMPLE,trainLabel);
        SVMModel->train(data);


        //calculate accuracy
        int correct = 0;
        for(int i=0;i<testData.size();i++)
        {
            if(SVMModel->predict(testData[i]) == testLabel[i])
                correct++;
        }
        double accuracy = (double)correct/(double)testData.size();
        qDebug() << accuracy;
        //            SVMModel->save("model_test.yaml");
        QString name = "model/model_";
        if(ui->actionWith_HOG->isChecked())
        {
            name = name+"HOG_";
        }
        if(ui->actionWith_PCA->isChecked())
        {
            name = name+"PCA_"+QString::number(ui->PCARemains_spinBox->value())+"_";
        }
        SVMModel->save((name+QString::number(j+C_lower)+"_"+QString::number(accuracy)+".yaml").toStdString());
        //            SVMModel->save((name+QString::number(j+C_lower)+"_"+QString::number(k+Gamma_lower)+"_"+QString::number(accuracy)+".yaml").toStdString());
        //        }
    }
}

