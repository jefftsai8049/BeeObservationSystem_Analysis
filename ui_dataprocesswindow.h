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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataProcessWindow
{
public:
    QAction *actionOpen_Raw_Data;
    QWidget *centralwidget;
    QPushButton *data_preprocessing_pushButton;
    QTextBrowser *system_log_textBrowser;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DataProcessWindow)
    {
        if (DataProcessWindow->objectName().isEmpty())
            DataProcessWindow->setObjectName(QStringLiteral("DataProcessWindow"));
        DataProcessWindow->resize(800, 600);
        actionOpen_Raw_Data = new QAction(DataProcessWindow);
        actionOpen_Raw_Data->setObjectName(QStringLiteral("actionOpen_Raw_Data"));
        centralwidget = new QWidget(DataProcessWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        data_preprocessing_pushButton = new QPushButton(centralwidget);
        data_preprocessing_pushButton->setObjectName(QStringLiteral("data_preprocessing_pushButton"));
        data_preprocessing_pushButton->setEnabled(false);
        data_preprocessing_pushButton->setGeometry(QRect(30, 20, 151, 61));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(14);
        data_preprocessing_pushButton->setFont(font);
        system_log_textBrowser = new QTextBrowser(centralwidget);
        system_log_textBrowser->setObjectName(QStringLiteral("system_log_textBrowser"));
        system_log_textBrowser->setGeometry(QRect(20, 440, 761, 111));
        DataProcessWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DataProcessWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        DataProcessWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DataProcessWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        DataProcessWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Raw_Data);

        retranslateUi(DataProcessWindow);

        QMetaObject::connectSlotsByName(DataProcessWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DataProcessWindow)
    {
        DataProcessWindow->setWindowTitle(QApplication::translate("DataProcessWindow", "Data Process", 0));
        actionOpen_Raw_Data->setText(QApplication::translate("DataProcessWindow", "Open Raw Data", 0));
        data_preprocessing_pushButton->setText(QApplication::translate("DataProcessWindow", "Preprocessing", 0));
        menuFile->setTitle(QApplication::translate("DataProcessWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class DataProcessWindow: public Ui_DataProcessWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAPROCESSWINDOW_H
