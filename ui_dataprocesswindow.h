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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataProcessWindow
{
public:
    QWidget *centralwidget;
    QPushButton *data_preprocessing_pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DataProcessWindow)
    {
        if (DataProcessWindow->objectName().isEmpty())
            DataProcessWindow->setObjectName(QStringLiteral("DataProcessWindow"));
        DataProcessWindow->resize(800, 600);
        centralwidget = new QWidget(DataProcessWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        data_preprocessing_pushButton = new QPushButton(centralwidget);
        data_preprocessing_pushButton->setObjectName(QStringLiteral("data_preprocessing_pushButton"));
        data_preprocessing_pushButton->setGeometry(QRect(100, 90, 151, 61));
        DataProcessWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DataProcessWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        DataProcessWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DataProcessWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        DataProcessWindow->setStatusBar(statusbar);

        retranslateUi(DataProcessWindow);

        QMetaObject::connectSlotsByName(DataProcessWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DataProcessWindow)
    {
        DataProcessWindow->setWindowTitle(QApplication::translate("DataProcessWindow", "Data Process", 0));
        data_preprocessing_pushButton->setText(QApplication::translate("DataProcessWindow", "Preprocessing", 0));
    } // retranslateUi

};

namespace Ui {
    class DataProcessWindow: public Ui_DataProcessWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAPROCESSWINDOW_H
