#include "dataprocesswindow.h"
#include "ui_dataprocesswindow.h"

DataProcessWindow::DataProcessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataProcessWindow)
{
    ui->setupUi(this);
    OT = new object_tracking;
    OTS = new ObjectTrackingForm;
    connect(this,SIGNAL(sendSystemLog(QString)),this,SLOT(receiveSystemLog(QString)));

    connect(OT,SIGNAL(sendSystemLog(QString)),this,SLOT(receiveSystemLog(QString)));
    connect(OT,SIGNAL(sendLoadRawDataFinish()),this,SLOT(receiveLoadDataFinish()));
    connect(OT,SIGNAL(sendProgress(int)),this,SLOT(receiveProgress(int)));

    connect(OTS,SIGNAL(setObjectTrackingParameters(objectTrackingParameters)),this,SLOT(setObjectTrackingParameters(objectTrackingParameters)));
    OTS->requestObjectTrackingParameters();
    OT->setPathSegmentSize(this->OTParams.segmentSize);
}

DataProcessWindow::~DataProcessWindow()
{
    delete ui;
}


void DataProcessWindow::on_data_preprocessing_pushButton_clicked()
{
    QFuture<void> rawDataProcessing = QtConcurrent::run(OT,&object_tracking::rawDataPreprocessing,&this->path,&this->data);
    rawDataProcessing.waitForFinished();
    this->path.clear();

}

void DataProcessWindow::receiveSystemLog(const QString &log)
{
    ui->system_log_textBrowser->append(log);
}

void DataProcessWindow::receiveLoadDataFinish()
{
    ui->data_preprocessing_pushButton->setEnabled(true);
}

void DataProcessWindow::receiveProgress(const int &val)
{
    ui->progressBar->setValue(val);
}

void DataProcessWindow::setObjectTrackingParameters(const objectTrackingParameters &params)
{
    OTParams = params;
    OT->setPathSegmentSize(this->OTParams.segmentSize);
}

void DataProcessWindow::on_actionOpen_Raw_Data_triggered()
{
    ui->data_preprocessing_pushButton->setEnabled(false);
    QStringList fileNames;
    fileNames = QFileDialog::getOpenFileNames(this,"Open Data File","","Data Files (*.csv)");
    if(fileNames.size() == 0)
        return;
    QFuture<void> loadDataFunction = QtConcurrent::run(OT,&object_tracking::loadDataTrack,fileNames,&this->path);
}

void DataProcessWindow::on_actionOpen_Processed_Data_triggered()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,"Open Data File","","Data Files (*.csv)");

    if(fileName.size() == 0)
        return;
    QFuture<void> loadDataTrackPro = QtConcurrent::run(OT,&object_tracking::loadDataTrackPro,fileName,&this->data);
    //OT->loadDataTrackPro(fileName,&this->data);
    loadDataTrackPro.waitForFinished();
    //qDebug() << this->data.size();

    ui->trajectory_classify_pushButton->setEnabled(true);

}

void DataProcessWindow::on_trajectory_classify_pushButton_clicked()
{
    OT->tracjectoryClassify(this->data,this->OTParams);
}

void DataProcessWindow::on_actionObject_Tracking_triggered()
{
    OTS->show();
}
