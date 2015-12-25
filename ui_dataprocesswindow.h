/********************************************************************************
** Form generated from reading UI file 'dataprocesswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAPROCESSWINDOW_H
#define UI_DATAPROCESSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataProcessWindow
{
public:
    QAction *actionOpen_Raw_Data;
    QAction *actionOpen_Processed_Data;
    QWidget *centralwidget;
    QTextBrowser *system_log_textBrowser;
    QProgressBar *progressBar;
    QGroupBox *groupBox;
    QPushButton *data_preprocessing_pushButton;
    QLabel *label;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QPushButton *trajectory_classify_pushButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DataProcessWindow)
    {
        if (DataProcessWindow->objectName().isEmpty())
            DataProcessWindow->setObjectName(QStringLiteral("DataProcessWindow"));
        DataProcessWindow->resize(800, 600);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(14);
        DataProcessWindow->setFont(font);
        actionOpen_Raw_Data = new QAction(DataProcessWindow);
        actionOpen_Raw_Data->setObjectName(QStringLiteral("actionOpen_Raw_Data"));
        actionOpen_Processed_Data = new QAction(DataProcessWindow);
        actionOpen_Processed_Data->setObjectName(QStringLiteral("actionOpen_Processed_Data"));
        centralwidget = new QWidget(DataProcessWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        system_log_textBrowser = new QTextBrowser(centralwidget);
        system_log_textBrowser->setObjectName(QStringLiteral("system_log_textBrowser"));
        system_log_textBrowser->setGeometry(QRect(20, 440, 761, 111));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(570, 330, 191, 91));
        progressBar->setValue(0);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 60, 221, 111));
        data_preprocessing_pushButton = new QPushButton(groupBox);
        data_preprocessing_pushButton->setObjectName(QStringLiteral("data_preprocessing_pushButton"));
        data_preprocessing_pushButton->setEnabled(false);
        data_preprocessing_pushButton->setGeometry(QRect(20, 30, 181, 61));
        data_preprocessing_pushButton->setFont(font);
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 211, 41));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 180, 271, 41));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 230, 221, 111));
        trajectory_classify_pushButton = new QPushButton(groupBox_2);
        trajectory_classify_pushButton->setObjectName(QStringLiteral("trajectory_classify_pushButton"));
        trajectory_classify_pushButton->setEnabled(false);
        trajectory_classify_pushButton->setGeometry(QRect(20, 30, 181, 61));
        DataProcessWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DataProcessWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 28));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        DataProcessWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DataProcessWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        DataProcessWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Raw_Data);
        menuFile->addAction(actionOpen_Processed_Data);

        retranslateUi(DataProcessWindow);

        QMetaObject::connectSlotsByName(DataProcessWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DataProcessWindow)
    {
        DataProcessWindow->setWindowTitle(QApplication::translate("DataProcessWindow", "Data Process", 0));
        actionOpen_Raw_Data->setText(QApplication::translate("DataProcessWindow", "Open Raw Data", 0));
        actionOpen_Processed_Data->setText(QApplication::translate("DataProcessWindow", "Open Processed Data", 0));
        groupBox->setTitle(QApplication::translate("DataProcessWindow", "Step 2.", 0));
        data_preprocessing_pushButton->setText(QApplication::translate("DataProcessWindow", "Preprocessing", 0));
        label->setText(QApplication::translate("DataProcessWindow", "Step 1. Open Raw Data", 0));
        label_2->setText(QApplication::translate("DataProcessWindow", "Step 3. Open Processed Data", 0));
        groupBox_2->setTitle(QApplication::translate("DataProcessWindow", "Step 4.", 0));
        trajectory_classify_pushButton->setText(QApplication::translate("DataProcessWindow", "Trajectory Classify", 0));
        menuFile->setTitle(QApplication::translate("DataProcessWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class DataProcessWindow: public Ui_DataProcessWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAPROCESSWINDOW_H
