#ifndef DATAPROCESSWINDOW_H
#define DATAPROCESSWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDateTime>

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

    bool loadData(const QStringList &fileNames);

private slots:
    void on_data_preprocessing_pushButton_clicked();

private:
    Ui::DataProcessWindow *ui;

    std::vector<track> path;

    std::vector<trackPro> data;

    QString voting(track &path);

};



#endif // DATAPROCESSWINDOW_H
