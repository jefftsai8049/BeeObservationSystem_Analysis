#include "dataprocesswindow.h"
#include "ui_dataprocesswindow.h"

DataProcessWindow::DataProcessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataProcessWindow)
{
    ui->setupUi(this);
    OT = new object_tracking;

    connect(this,SIGNAL(sendSystemLog(QString)),this,SLOT(receiveSystemLog(QString)));


    connect(OT,SIGNAL(sendSystemLog(QString)),this,SLOT(receiveSystemLog(QString)));
    connect(OT,SIGNAL(sendLoadRawDataFinish()),this,SLOT(receiveLoadDataFinish()));

}

DataProcessWindow::~DataProcessWindow()
{
    delete ui;
}

void DataProcessWindow::loadData(const QStringList &fileNames)
{

    for(int i = 0; i < fileNames.size(); i++)
    {
        QFile f;
        f.setFileName(fileNames[i]);
        emit sendSystemLog(fileNames[i]);
        f.open(QIODevice::ReadOnly);
        while(!f.atEnd())
        {
            track t;
            QString msg = f.readLine();
            msg = msg.trimmed();
            while(msg.at(msg.size()-1) == ",")
                msg = msg.left(msg.size()-1);

            QStringList data = msg.split(",");
            for(int m = 0; m < data.size(); m=m+5)
            {
                t.w1.push_back(data[m].toStdString()[0]);
                t.w2.push_back(data[m+1].toStdString()[0]);
                t.time.push_back(QDateTime::fromString(data[m+2],"yyyy-MM-dd_hh-mm-ss-zzz"));
                t.position.push_back(cv::Point(data[m+3].toInt(),data[m+4].toInt()));
            }
            this->path.push_back(t);

        }
    }
}

void DataProcessWindow::on_data_preprocessing_pushButton_clicked()
{
    QVector<trackPro> TPVector;
    OT->rawDataPreprocessing(&this->path,&TPVector);
    qDebug() << TPVector.size();
//    QVector<trackPro> TPVector;
//    for(int i = 0; i < path.size(); i++)
//    {
//        trackPro TP;
////        qDebug() << "preprocessing" << i;
//        TP.ID = OT->voting(this->path[i]);
//        TP.startTime = this->path[i].time[0];
//        TP.endTime = this->path[i].time[this->path[i].size()-1];
//        TP.position = OT->interpolation(this->path[i].position,this->path[i].time);
//        TP.size = TP.position.size();
////        qDebug() << TP.ID << TP.startTime << TP.endTime << TP.size();

//        TPVector.append(TP);
//    }

//    //check dir is exist or not
//    QDir dir("processed_data");
//    if(!dir.exists())
//    {
//        dir.cdUp();
//        if(dir.mkdir("processed_data"))
//        {
//            dir.cd("processed_data");
//        }
//        else
//        {
//            emit sendSystemLog("Create dir failed!");
//            return;
//        }
//    }

//    //open file for save sensor data
//    QString fileName = dir.absolutePath()+"/"+TPVector.at(0).startTime.toString("yyyy-MM-dd_hh-mm-ss-zzz")+"_processed"+".csv";
//    OT->saveTrackPro(TPVector,fileName);
}

void DataProcessWindow::receiveSystemLog(const QString &log)
{
    ui->system_log_textBrowser->append(log);
}

void DataProcessWindow::receiveLoadDataFinish()
{
    ui->data_preprocessing_pushButton->setEnabled(true);
}

void DataProcessWindow::on_actionOpen_Raw_Data_triggered()
{
    QStringList fileNames;
    fileNames = QFileDialog::getOpenFileNames(this,"Open Data File","","Data Files (*.csv)");
    QFuture<void> loadDataFunction = QtConcurrent::run(OT,&object_tracking::loadDataTrackPro,fileNames,&this->path);
}
