/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Raw_Video_File;
    QAction *actionLoad_Stitching_Image;
    QAction *actionLoad_Maunal_Stitching_Setting;
    QWidget *centralWidget;
    QTextBrowser *videoName_textBrowser;
    QPushButton *stitchingStart_pushButton;
    QPushButton *stitchingStop_pushButton;
    QLabel *label;
    QPushButton *stitching_pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(760, 434);
        QFont font;
        font.setFamily(QStringLiteral("Noto Sans CJK TC DemiLight"));
        font.setPointSize(14);
        MainWindow->setFont(font);
        actionLoad_Raw_Video_File = new QAction(MainWindow);
        actionLoad_Raw_Video_File->setObjectName(QStringLiteral("actionLoad_Raw_Video_File"));
        actionLoad_Raw_Video_File->setCheckable(false);
        actionLoad_Stitching_Image = new QAction(MainWindow);
        actionLoad_Stitching_Image->setObjectName(QStringLiteral("actionLoad_Stitching_Image"));
        actionLoad_Maunal_Stitching_Setting = new QAction(MainWindow);
        actionLoad_Maunal_Stitching_Setting->setObjectName(QStringLiteral("actionLoad_Maunal_Stitching_Setting"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        videoName_textBrowser = new QTextBrowser(centralWidget);
        videoName_textBrowser->setObjectName(QStringLiteral("videoName_textBrowser"));
        videoName_textBrowser->setGeometry(QRect(410, 40, 221, 311));
        QFont font1;
        font1.setPointSize(10);
        videoName_textBrowser->setFont(font1);
        stitchingStart_pushButton = new QPushButton(centralWidget);
        stitchingStart_pushButton->setObjectName(QStringLiteral("stitchingStart_pushButton"));
        stitchingStart_pushButton->setGeometry(QRect(30, 260, 151, 91));
        stitchingStop_pushButton = new QPushButton(centralWidget);
        stitchingStop_pushButton->setObjectName(QStringLiteral("stitchingStop_pushButton"));
        stitchingStop_pushButton->setGeometry(QRect(190, 260, 151, 91));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(410, -1, 141, 41));
        stitching_pushButton = new QPushButton(centralWidget);
        stitching_pushButton->setObjectName(QStringLiteral("stitching_pushButton"));
        stitching_pushButton->setGeometry(QRect(30, 160, 151, 91));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 760, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad_Stitching_Image);
        menuFile->addAction(actionLoad_Maunal_Stitching_Setting);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_Raw_Video_File);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionLoad_Raw_Video_File->setText(QApplication::translate("MainWindow", "Load Raw Video File", 0));
        actionLoad_Stitching_Image->setText(QApplication::translate("MainWindow", "Load Stitching Image", 0));
        actionLoad_Maunal_Stitching_Setting->setText(QApplication::translate("MainWindow", "Load Maunal Stitching Setting", 0));
        stitchingStart_pushButton->setText(QApplication::translate("MainWindow", "Start", 0));
        stitchingStop_pushButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        label->setText(QApplication::translate("MainWindow", "File Sequence", 0));
        stitching_pushButton->setText(QApplication::translate("MainWindow", "Stitch", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
