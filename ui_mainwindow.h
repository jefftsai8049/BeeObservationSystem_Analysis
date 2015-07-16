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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *stitching_pushButton;
    QPushButton *stitchingStart_pushButton;
    QPushButton *stitchingStop_pushButton;
    QLabel *label;
    QTextBrowser *videoName_textBrowser;
    QLCDNumber *processing_lcdNumber;
    QCheckBox *show_image_checkBox;
    QPushButton *cuda_test_pushButton;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QSpinBox *maxRadius_hough_circle_spinBox;
    QSpinBox *dp_hough_circle_spinBox;
    QSpinBox *para_2_hough_circle_spinBox;
    QSpinBox *minRadius_hough_circle_spinBox;
    QSpinBox *para_1_hough_circle_spinBox;
    QSpinBox *minDist_hough_circle_spinBox;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QWidget *tab_3;
    QPushButton *load_training_data_pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(607, 462);
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
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 0, 591, 401));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        stitching_pushButton = new QPushButton(tab);
        stitching_pushButton->setObjectName(QStringLiteral("stitching_pushButton"));
        stitching_pushButton->setGeometry(QRect(10, 150, 151, 91));
        stitchingStart_pushButton = new QPushButton(tab);
        stitchingStart_pushButton->setObjectName(QStringLiteral("stitchingStart_pushButton"));
        stitchingStart_pushButton->setGeometry(QRect(10, 260, 151, 91));
        stitchingStop_pushButton = new QPushButton(tab);
        stitchingStop_pushButton->setObjectName(QStringLiteral("stitchingStop_pushButton"));
        stitchingStop_pushButton->setGeometry(QRect(180, 260, 151, 91));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(350, 0, 141, 41));
        videoName_textBrowser = new QTextBrowser(tab);
        videoName_textBrowser->setObjectName(QStringLiteral("videoName_textBrowser"));
        videoName_textBrowser->setGeometry(QRect(350, 41, 221, 311));
        QFont font1;
        font1.setPointSize(10);
        videoName_textBrowser->setFont(font1);
        processing_lcdNumber = new QLCDNumber(tab);
        processing_lcdNumber->setObjectName(QStringLiteral("processing_lcdNumber"));
        processing_lcdNumber->setGeometry(QRect(30, 20, 101, 51));
        show_image_checkBox = new QCheckBox(tab);
        show_image_checkBox->setObjectName(QStringLiteral("show_image_checkBox"));
        show_image_checkBox->setGeometry(QRect(190, 190, 141, 51));
        show_image_checkBox->setChecked(true);
        cuda_test_pushButton = new QPushButton(tab);
        cuda_test_pushButton->setObjectName(QStringLiteral("cuda_test_pushButton"));
        cuda_test_pushButton->setGeometry(QRect(180, 40, 151, 91));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 211, 341));
        maxRadius_hough_circle_spinBox = new QSpinBox(groupBox);
        maxRadius_hough_circle_spinBox->setObjectName(QStringLiteral("maxRadius_hough_circle_spinBox"));
        maxRadius_hough_circle_spinBox->setGeometry(QRect(100, 140, 61, 31));
        maxRadius_hough_circle_spinBox->setValue(19);
        dp_hough_circle_spinBox = new QSpinBox(groupBox);
        dp_hough_circle_spinBox->setObjectName(QStringLiteral("dp_hough_circle_spinBox"));
        dp_hough_circle_spinBox->setGeometry(QRect(10, 60, 61, 31));
        dp_hough_circle_spinBox->setValue(2);
        para_2_hough_circle_spinBox = new QSpinBox(groupBox);
        para_2_hough_circle_spinBox->setObjectName(QStringLiteral("para_2_hough_circle_spinBox"));
        para_2_hough_circle_spinBox->setGeometry(QRect(10, 300, 61, 31));
        para_2_hough_circle_spinBox->setMaximum(200);
        para_2_hough_circle_spinBox->setValue(35);
        minRadius_hough_circle_spinBox = new QSpinBox(groupBox);
        minRadius_hough_circle_spinBox->setObjectName(QStringLiteral("minRadius_hough_circle_spinBox"));
        minRadius_hough_circle_spinBox->setGeometry(QRect(100, 60, 61, 31));
        minRadius_hough_circle_spinBox->setValue(12);
        para_1_hough_circle_spinBox = new QSpinBox(groupBox);
        para_1_hough_circle_spinBox->setObjectName(QStringLiteral("para_1_hough_circle_spinBox"));
        para_1_hough_circle_spinBox->setGeometry(QRect(10, 220, 61, 31));
        para_1_hough_circle_spinBox->setMaximum(200);
        para_1_hough_circle_spinBox->setValue(150);
        minDist_hough_circle_spinBox = new QSpinBox(groupBox);
        minDist_hough_circle_spinBox->setObjectName(QStringLiteral("minDist_hough_circle_spinBox"));
        minDist_hough_circle_spinBox->setGeometry(QRect(10, 140, 61, 31));
        minDist_hough_circle_spinBox->setValue(20);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 41, 31));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 190, 31, 31));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 270, 31, 31));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 110, 81, 31));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(100, 30, 101, 31));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(100, 110, 101, 31));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        load_training_data_pushButton = new QPushButton(tab_3);
        load_training_data_pushButton->setObjectName(QStringLiteral("load_training_data_pushButton"));
        load_training_data_pushButton->setGeometry(QRect(10, 10, 181, 81));
        tabWidget->addTab(tab_3, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 607, 21));
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

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionLoad_Raw_Video_File->setText(QApplication::translate("MainWindow", "Load Raw Video File", 0));
        actionLoad_Stitching_Image->setText(QApplication::translate("MainWindow", "Load Stitching Image", 0));
        actionLoad_Maunal_Stitching_Setting->setText(QApplication::translate("MainWindow", "Load Maunal Stitching Setting", 0));
        stitching_pushButton->setText(QApplication::translate("MainWindow", "Stitch", 0));
        stitchingStart_pushButton->setText(QApplication::translate("MainWindow", "Start", 0));
        stitchingStop_pushButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        label->setText(QApplication::translate("MainWindow", "File Sequence", 0));
        show_image_checkBox->setText(QApplication::translate("MainWindow", "Show Image", 0));
        cuda_test_pushButton->setText(QApplication::translate("MainWindow", "Cuda Test", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Main", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Hough Circle", 0));
        label_2->setText(QApplication::translate("MainWindow", "dp", 0));
        label_3->setText(QApplication::translate("MainWindow", "P1", 0));
        label_4->setText(QApplication::translate("MainWindow", "P2", 0));
        label_5->setText(QApplication::translate("MainWindow", "minDist", 0));
        label_6->setText(QApplication::translate("MainWindow", "minRadius", 0));
        label_7->setText(QApplication::translate("MainWindow", "maxRadius", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Setting", 0));
        load_training_data_pushButton->setText(QApplication::translate("MainWindow", "Load Data", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Tag", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
