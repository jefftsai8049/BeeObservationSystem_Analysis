/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
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
#include "qsmartgraphicsview/qsmartgraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Raw_Video_File;
    QAction *actionLoad_Stitching_Image;
    QAction *actionChange_SVM_Model;
    QAction *actionChange_PCA_Model;
    QAction *actionChange_Stitching_Model;
    QAction *actionWith_PCA;
    QAction *actionWith_HOG;
    QAction *actionTrain_New_Tag_Model;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *stitching_pushButton;
    QLabel *label;
    QTextBrowser *videoName_textBrowser;
    QLCDNumber *processing_lcdNumber;
    QCheckBox *show_image_checkBox;
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
    QGroupBox *groupBox_2;
    QSpinBox *binarythreshold_spinBox;
    QLabel *label_8;
    QWidget *tab_3;
    QPushButton *load_training_data_pushButton;
    QPushButton *test_recognition_pushButton;
    QSpinBox *PCARemains_spinBox;
    QPushButton *stitchingStart_pushButton;
    QPushButton *stitchingStop_pushButton;
    QSmartGraphicsView *imageShow_widget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSet;
    QMenu *menuModel;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1344, 640);
        QFont font;
        font.setFamily(QStringLiteral("Noto Sans CJK TC DemiLight"));
        font.setPointSize(14);
        MainWindow->setFont(font);
        actionLoad_Raw_Video_File = new QAction(MainWindow);
        actionLoad_Raw_Video_File->setObjectName(QStringLiteral("actionLoad_Raw_Video_File"));
        actionLoad_Raw_Video_File->setCheckable(false);
        actionLoad_Stitching_Image = new QAction(MainWindow);
        actionLoad_Stitching_Image->setObjectName(QStringLiteral("actionLoad_Stitching_Image"));
        actionChange_SVM_Model = new QAction(MainWindow);
        actionChange_SVM_Model->setObjectName(QStringLiteral("actionChange_SVM_Model"));
        actionChange_PCA_Model = new QAction(MainWindow);
        actionChange_PCA_Model->setObjectName(QStringLiteral("actionChange_PCA_Model"));
        actionChange_Stitching_Model = new QAction(MainWindow);
        actionChange_Stitching_Model->setObjectName(QStringLiteral("actionChange_Stitching_Model"));
        actionWith_PCA = new QAction(MainWindow);
        actionWith_PCA->setObjectName(QStringLiteral("actionWith_PCA"));
        actionWith_PCA->setCheckable(true);
        actionWith_PCA->setChecked(true);
        actionWith_HOG = new QAction(MainWindow);
        actionWith_HOG->setObjectName(QStringLiteral("actionWith_HOG"));
        actionWith_HOG->setCheckable(true);
        actionWith_HOG->setChecked(true);
        actionTrain_New_Tag_Model = new QAction(MainWindow);
        actionTrain_New_Tag_Model->setObjectName(QStringLiteral("actionTrain_New_Tag_Model"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1080, 10, 251, 501));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        stitching_pushButton = new QPushButton(tab);
        stitching_pushButton->setObjectName(QStringLiteral("stitching_pushButton"));
        stitching_pushButton->setGeometry(QRect(10, 10, 101, 51));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 110, 141, 41));
        videoName_textBrowser = new QTextBrowser(tab);
        videoName_textBrowser->setObjectName(QStringLiteral("videoName_textBrowser"));
        videoName_textBrowser->setGeometry(QRect(10, 151, 221, 311));
        QFont font1;
        font1.setPointSize(10);
        videoName_textBrowser->setFont(font1);
        processing_lcdNumber = new QLCDNumber(tab);
        processing_lcdNumber->setObjectName(QStringLiteral("processing_lcdNumber"));
        processing_lcdNumber->setGeometry(QRect(120, 9, 101, 51));
        show_image_checkBox = new QCheckBox(tab);
        show_image_checkBox->setObjectName(QStringLiteral("show_image_checkBox"));
        show_image_checkBox->setGeometry(QRect(120, 69, 111, 51));
        show_image_checkBox->setChecked(true);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 211, 251));
        maxRadius_hough_circle_spinBox = new QSpinBox(groupBox);
        maxRadius_hough_circle_spinBox->setObjectName(QStringLiteral("maxRadius_hough_circle_spinBox"));
        maxRadius_hough_circle_spinBox->setGeometry(QRect(100, 130, 61, 31));
        maxRadius_hough_circle_spinBox->setValue(9);
        dp_hough_circle_spinBox = new QSpinBox(groupBox);
        dp_hough_circle_spinBox->setObjectName(QStringLiteral("dp_hough_circle_spinBox"));
        dp_hough_circle_spinBox->setGeometry(QRect(10, 60, 61, 31));
        dp_hough_circle_spinBox->setValue(2);
        para_2_hough_circle_spinBox = new QSpinBox(groupBox);
        para_2_hough_circle_spinBox->setObjectName(QStringLiteral("para_2_hough_circle_spinBox"));
        para_2_hough_circle_spinBox->setGeometry(QRect(100, 200, 61, 31));
        para_2_hough_circle_spinBox->setMaximum(200);
        para_2_hough_circle_spinBox->setValue(25);
        minRadius_hough_circle_spinBox = new QSpinBox(groupBox);
        minRadius_hough_circle_spinBox->setObjectName(QStringLiteral("minRadius_hough_circle_spinBox"));
        minRadius_hough_circle_spinBox->setGeometry(QRect(100, 60, 61, 31));
        minRadius_hough_circle_spinBox->setValue(6);
        para_1_hough_circle_spinBox = new QSpinBox(groupBox);
        para_1_hough_circle_spinBox->setObjectName(QStringLiteral("para_1_hough_circle_spinBox"));
        para_1_hough_circle_spinBox->setGeometry(QRect(10, 200, 61, 31));
        para_1_hough_circle_spinBox->setMaximum(200);
        para_1_hough_circle_spinBox->setValue(150);
        minDist_hough_circle_spinBox = new QSpinBox(groupBox);
        minDist_hough_circle_spinBox->setObjectName(QStringLiteral("minDist_hough_circle_spinBox"));
        minDist_hough_circle_spinBox->setGeometry(QRect(10, 130, 61, 31));
        minDist_hough_circle_spinBox->setValue(20);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 41, 31));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 170, 31, 31));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(100, 170, 31, 31));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 100, 81, 31));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(100, 30, 101, 31));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(100, 100, 101, 31));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 280, 211, 171));
        binarythreshold_spinBox = new QSpinBox(groupBox_2);
        binarythreshold_spinBox->setObjectName(QStringLiteral("binarythreshold_spinBox"));
        binarythreshold_spinBox->setGeometry(QRect(10, 60, 61, 31));
        binarythreshold_spinBox->setMaximum(255);
        binarythreshold_spinBox->setValue(110);
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 30, 121, 21));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        load_training_data_pushButton = new QPushButton(tab_3);
        load_training_data_pushButton->setObjectName(QStringLiteral("load_training_data_pushButton"));
        load_training_data_pushButton->setGeometry(QRect(10, 20, 181, 81));
        test_recognition_pushButton = new QPushButton(tab_3);
        test_recognition_pushButton->setObjectName(QStringLiteral("test_recognition_pushButton"));
        test_recognition_pushButton->setGeometry(QRect(10, 120, 181, 81));
        PCARemains_spinBox = new QSpinBox(tab_3);
        PCARemains_spinBox->setObjectName(QStringLiteral("PCARemains_spinBox"));
        PCARemains_spinBox->setGeometry(QRect(30, 270, 101, 31));
        PCARemains_spinBox->setMinimum(1);
        PCARemains_spinBox->setMaximum(256);
        PCARemains_spinBox->setValue(25);
        tabWidget->addTab(tab_3, QString());
        stitchingStart_pushButton = new QPushButton(centralWidget);
        stitchingStart_pushButton->setObjectName(QStringLiteral("stitchingStart_pushButton"));
        stitchingStart_pushButton->setGeometry(QRect(1090, 520, 101, 51));
        stitchingStop_pushButton = new QPushButton(centralWidget);
        stitchingStop_pushButton->setObjectName(QStringLiteral("stitchingStop_pushButton"));
        stitchingStop_pushButton->setGeometry(QRect(1220, 520, 101, 51));
        imageShow_widget = new QSmartGraphicsView(centralWidget);
        imageShow_widget->setObjectName(QStringLiteral("imageShow_widget"));
        imageShow_widget->setGeometry(QRect(10, 10, 1061, 561));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1344, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuSet = new QMenu(menuBar);
        menuSet->setObjectName(QStringLiteral("menuSet"));
        menuModel = new QMenu(menuBar);
        menuModel->setObjectName(QStringLiteral("menuModel"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuSet->menuAction());
        menuBar->addAction(menuModel->menuAction());
        menuFile->addAction(actionLoad_Stitching_Image);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_Raw_Video_File);
        menuSet->addAction(actionChange_SVM_Model);
        menuSet->addAction(actionChange_PCA_Model);
        menuSet->addAction(actionChange_Stitching_Model);
        menuSet->addSeparator();
        menuSet->addAction(actionWith_PCA);
        menuSet->addAction(actionWith_HOG);
        menuModel->addAction(actionTrain_New_Tag_Model);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "NTU BIME Honeybee Observation System", 0));
        actionLoad_Raw_Video_File->setText(QApplication::translate("MainWindow", "Load Raw Video File", 0));
        actionLoad_Stitching_Image->setText(QApplication::translate("MainWindow", "Load Stitching Image", 0));
        actionChange_SVM_Model->setText(QApplication::translate("MainWindow", "Change SVM Model", 0));
        actionChange_PCA_Model->setText(QApplication::translate("MainWindow", "Change PCA Model", 0));
        actionChange_Stitching_Model->setText(QApplication::translate("MainWindow", "Change Stitching Model", 0));
        actionWith_PCA->setText(QApplication::translate("MainWindow", "With PCA", 0));
        actionWith_HOG->setText(QApplication::translate("MainWindow", "With HOG", 0));
        actionTrain_New_Tag_Model->setText(QApplication::translate("MainWindow", "Train New Tag Model", 0));
        stitching_pushButton->setText(QApplication::translate("MainWindow", "Stitch", 0));
        label->setText(QApplication::translate("MainWindow", "File Sequence", 0));
        show_image_checkBox->setText(QApplication::translate("MainWindow", "Show Image", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Main", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Hough Circle", 0));
        label_2->setText(QApplication::translate("MainWindow", "dp", 0));
        label_3->setText(QApplication::translate("MainWindow", "P1", 0));
        label_4->setText(QApplication::translate("MainWindow", "P2", 0));
        label_5->setText(QApplication::translate("MainWindow", "minDist", 0));
        label_6->setText(QApplication::translate("MainWindow", "minRadius", 0));
        label_7->setText(QApplication::translate("MainWindow", "maxRadius", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Tag Recogntion", 0));
        label_8->setText(QApplication::translate("MainWindow", "Binary Threhold", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Setting", 0));
        load_training_data_pushButton->setText(QApplication::translate("MainWindow", "Train", 0));
        test_recognition_pushButton->setText(QApplication::translate("MainWindow", "Test", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Tag", 0));
        stitchingStart_pushButton->setText(QApplication::translate("MainWindow", "Start", 0));
        stitchingStop_pushButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuSet->setTitle(QApplication::translate("MainWindow", "Setting", 0));
        menuModel->setTitle(QApplication::translate("MainWindow", "Model", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
