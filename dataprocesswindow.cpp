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

void DataProcessWindow::on_actionOpen_Sensor_Data_triggered()
{
    QStringList fileNames;
    fileNames = QFileDialog::getOpenFileNames(this,"Open Sensor File","","Data Files (*.csv)");

    QVector<weatherInfo> weatherData;
    try
    {
        this->loadWeatherData(fileNames,weatherData);
    }
    catch(const QString &errMsg)
    {
        ui->system_log_textBrowser->insertPlainText(errMsg);
    }

    QVector<double> x1,y1;
    this->getInHiveRH(weatherData,x1,y1);
    ui->weather_info_widget->addGraph();
    ui->weather_info_widget->graph(0)->setData(x1,y1);
    //ui->weather_info_widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,QPen(Qt::black,1.5), QBrush(Qt::white), 9));
    ui->weather_info_widget->graph(0)->setPen(QPen(QColor(255,0,0),2));
    ui->weather_info_widget->graph(0)->setName("In-Hive Humidity");


    QVector<double> x2,y2;
    this->getOutHiveRH(weatherData,x2,y2);
    ui->weather_info_widget->addGraph();
    ui->weather_info_widget->graph(1)->setData(x2,y2);
    //ui->weather_info_widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,QPen(Qt::black,1.5), QBrush(Qt::white), 9));
    ui->weather_info_widget->graph(1)->setPen(QPen(QColor(0,0,255),2));
    ui->weather_info_widget->graph(1)->setName("Out-Hive Humidity");

    ui->weather_info_widget->xAxis->setRange(x2.at(0),x2.at(x2.size()-1));
    ui->weather_info_widget->yAxis->setRange(30,120);


    QVector<double> x3,y3;
    this->getInHiveTemp(weatherData,x3,y3);
    ui->weather_info_widget->addGraph(ui->weather_info_widget->xAxis2,ui->weather_info_widget->yAxis2);
    ui->weather_info_widget->graph(2)->setData(x3,y3);
    //ui->weather_info_widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,QPen(Qt::black,1.5), QBrush(Qt::white), 9));
    ui->weather_info_widget->graph(2)->setPen(QPen(QColor(Qt::green),2));
    ui->weather_info_widget->graph(2)->setName("In-Hive Temperture");

    QVector<double> x4,y4;
    this->getOutHiveTemp(weatherData,x4,y4);
    ui->weather_info_widget->addGraph(ui->weather_info_widget->xAxis2,ui->weather_info_widget->yAxis2);
    ui->weather_info_widget->graph(3)->setData(x4,y4);
    //ui->weather_info_widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,QPen(Qt::black,1.5), QBrush(Qt::white), 9));
    ui->weather_info_widget->graph(3)->setPen(QPen(QColor(Qt::darkCyan),2));
    ui->weather_info_widget->graph(3)->setName("Out-Hive Temperture");

//    QVector<double> x3,y3;
//    this->getInHiveTemp(weatherData,x3,y3);
//    QCPBars *inTemp = new QCPBars(ui->weather_info_widget->xAxis2,ui->weather_info_widget->yAxis2);
//    inTemp->setData(x3,y3);
//    inTemp->setPen(QPen(QColor(0,0,255),2));
//    inTemp->setName("In-Hive Temperture");
//    ui->weather_info_widget->addPlottable(inTemp);

//    QVector<double> x4,y4;
//    this->getInHiveTemp(weatherData,x4,y4);
//    QCPBars *outTemp = new QCPBars(ui->weather_info_widget->xAxis2,ui->weather_info_widget->yAxis2);
//    outTemp->setData(x4,y4);
//    outTemp->setPen(QPen(QColor(255,0,0),2));
//    outTemp->setName("Out-Hive Temperture");
//    ui->weather_info_widget->addPlottable(outTemp);

    ui->weather_info_widget->xAxis2->setRange(x3.at(0),x3.at(x3.size()-1));
    ui->weather_info_widget->yAxis2->setRange(10,40);
    ui->weather_info_widget->yAxis2->setVisible(true);

    ui->weather_info_widget->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignBottom);
    ui->weather_info_widget->legend->setVisible(true);

    ui->weather_info_widget->replot();

    for(int i = 0; i < x1.size(); i++)
    {
        qDebug() << x1.at(i) << y1.at(i);
    }
}

void DataProcessWindow::loadWeatherData(const QStringList &fileNames, QVector<weatherInfo> &weatherData)
{
    for(int i = 0; i < fileNames.size(); i++)
    {
        QFile file(fileNames.at(i));
        if(!file.exists())
        {
            throw fileNames.at(i)+QString(" file not exist");
            return;
        }
        weatherData.clear();
        file.open(QIODevice::ReadOnly);
        while(!file.atEnd())
        {
            QString msg = file.readLine();
            msg = msg.trimmed();
            QStringList data = msg.split(",");
            qDebug() << data[0]<< data[1]<< data[2]<< data[3]<< data[4]<< data[5];
            if(data.at(0)!="")
            {
                weatherInfo wData;

                wData.time = QDateTime::fromString(data.at(0),"yyyy-MM-dd_hh-mm-ss-zzz");
                wData.inHiveTemp = data.at(1).toFloat();
                wData.inHiveRH = data.at(2).toFloat();
                wData.outHiveTemp = data.at(3).toFloat();
                wData.outHiveRH = data.at(4).toFloat();
                wData.pressure = data.at(5).toFloat();

                weatherData.push_back(wData);
            }
        }
    }
}

void DataProcessWindow::getInHiveTemp(const QVector<weatherInfo> &weatherData, QVector<double> &x, QVector<double> &y)
{
    x.clear();
    y.clear();

    x.resize(weatherData.size());
    y.resize(weatherData.size());

    for(int i = 0; i < weatherData.size(); i++)
    {
        x[i] = i;
        y[i] = weatherData.at(i).inHiveTemp;
    }
}

void DataProcessWindow::getInHiveRH(const QVector<weatherInfo> &weatherData, QVector<double> &x, QVector<double> &y)
{
    x.clear();
    y.clear();

    x.resize(weatherData.size());
    y.resize(weatherData.size());

    for(int i = 0; i < weatherData.size(); i++)
    {
        x[i] = i;
        y[i] = weatherData.at(i).inHiveRH;
    }
}

void DataProcessWindow::getOutHiveTemp(const QVector<weatherInfo> &weatherData, QVector<double> &x, QVector<double> &y)
{
    x.clear();
    y.clear();

    x.resize(weatherData.size());
    y.resize(weatherData.size());

    for(int i = 0; i < weatherData.size(); i++)
    {
        x[i] = i;
        y[i] = weatherData.at(i).outHiveTemp;
    }
}

void DataProcessWindow::getOutHiveRH(const QVector<weatherInfo> &weatherData, QVector<double> &x, QVector<double> &y)
{
    x.clear();
    y.clear();

    x.resize(weatherData.size());
    y.resize(weatherData.size());

    for(int i = 0; i < weatherData.size(); i++)
    {
        x[i] = i;
        y[i] = weatherData.at(i).outHiveRH;
    }
}

void DataProcessWindow::getPressure(const QVector<weatherInfo> &weatherData, QVector<double> &x, QVector<double> &y)
{
    x.clear();
    y.clear();

    x.resize(weatherData.size());
    y.resize(weatherData.size());

    for(int i = 0; i < weatherData.size(); i++)
    {
        x[i] = i;
        y[i] = weatherData.at(i).pressure;
    }
}
