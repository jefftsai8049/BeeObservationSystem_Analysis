#ifndef DATAPROCESSWINDOW_H
#define DATAPROCESSWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QtConcurrent>



#include "object_tracking.h"

#include <opencv.hpp>



namespace Ui {
class DataProcessWindow;
}

class DataProcessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataProcessWindow(QWidget *parent = 0);
    ~DataProcessWindow();


private slots:
    void on_data_preprocessing_pushButton_clicked();

    void on_actionOpen_Raw_Data_triggered();

    void receiveSystemLog(const QString &log);

    void receiveLoadDataFinish();

    void receiveProgress(const int &val);

    void on_actionOpen_Processed_Data_triggered();

    void on_trajectory_classify_pushButton_clicked();

private:
    Ui::DataProcessWindow *ui;

    object_tracking *OT;

    std::vector<track> path;

    QVector<trackPro> data;

//    void rawDataPreprocessing();


signals:
    void sendSystemLog(const QString &log);


};



#endif // DATAPROCESSWINDOW_H
