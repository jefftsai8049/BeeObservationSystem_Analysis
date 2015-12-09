#include "dataprocesswindow.h"
#include "ui_dataprocesswindow.h"

DataProcessWindow::DataProcessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataProcessWindow)
{
    ui->setupUi(this);
}

DataProcessWindow::~DataProcessWindow()
{
    delete ui;
}

bool DataProcessWindow::loadData(const QStringList &fileNames)
{

    for(int i = 0; i < fileNames.size(); i++)
    {
        QFile f;
        f.setFileName(fileNames[i]);
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
    return true;
}

void DataProcessWindow::on_data_preprocessing_pushButton_clicked()
{

    for(int i = 0; i < path.size(); i++)
    {
        trackPro TP;
        qDebug() << "preprocessing" << i;
        TP.ID = this->voting(this->path[i]);
        TP.startTime = this->path[i].time[0];
        TP.endTime = this->path[i].time[this->path[i].size()-1];
        qDebug() << TP.ID << TP.startTime << TP.endTime;
    }

}

QString DataProcessWindow::voting(track &path)
{

    std::vector<char> w1_word;
    std::vector<int> w1_count;
    std::vector<char> w2_word;
    std::vector<int> w2_count;

    for(int i = 0; i < path.w1.size(); i++)
    {
        bool fined = false;
        for(int j = 0; j < w1_word.size(); j++)
        {
            if(w1_word[j] == path.w1[i])
            {
                fined = true;
                w1_count[j]++;
                break;
            }
        }
        if(!fined)
        {
            w1_word.push_back(path.w1[i]);
            w1_count.push_back(1);
        }

        fined = false;
        for(int j = 0; j < w2_word.size(); j++)
        {
            if(w2_word[j] == path.w2[i])
            {
                fined = true;
                w2_count[j]++;
                break;
            }
        }
        if(!fined)
        {
            w2_word.push_back(path.w2[i]);
            w2_count.push_back(1);
        }
    }

    for(int i = 0; i < w1_word.size()-1; i++)
    {
        for(int j = i+1; j < w1_word.size(); j++)
        {
            if(w1_count[i]<w1_count[j])
            {
                std::swap(w1_count[i],w1_count[j]);
                std::swap(w1_word[i],w1_word[j]);
            }
        }
    }

    for(int i = 0; i < w2_word.size()-1; i++)
    {
        for(int j = i+1; j < w2_word.size(); j++)
        {
            if(w2_count[i]<w2_count[j])
            {
                std::swap(w2_count[i],w2_count[j]);
                std::swap(w2_word[i],w2_word[j]);
            }
        }
    }

    char word1_final,word2_final;;

    for(int m = 0; m < w1_word.size(); m++)
    {
        if(w1_word[m] != '!')
        {
            word1_final = w1_word[m];
            break;
        }

    }

    for(int m = 0; m < w2_word.size(); m++)
    {
        if(w2_word[m] != '!')
        {
            word2_final = w2_word[m];
            break;
        }

    }
    QString result;
    result.push_back(word1_final);
    result.push_back(word2_final);
    return result;
}
