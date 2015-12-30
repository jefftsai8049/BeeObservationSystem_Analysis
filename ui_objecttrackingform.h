/********************************************************************************
** Form generated from reading UI file 'objecttrackingform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTTRACKINGFORM_H
#define UI_OBJECTTRACKINGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ObjectTrackingForm
{
public:
    QGroupBox *groupBox;
    QSpinBox *no_move_spinBox;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *loitering_move_spinBox;
    QLabel *label_3;
    QSpinBox *direction_spinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *ObjectTrackingForm)
    {
        if (ObjectTrackingForm->objectName().isEmpty())
            ObjectTrackingForm->setObjectName(QStringLiteral("ObjectTrackingForm"));
        ObjectTrackingForm->resize(243, 272);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(14);
        ObjectTrackingForm->setFont(font);
        groupBox = new QGroupBox(ObjectTrackingForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 221, 181));
        no_move_spinBox = new QSpinBox(groupBox);
        no_move_spinBox->setObjectName(QStringLiteral("no_move_spinBox"));
        no_move_spinBox->setGeometry(QRect(100, 40, 61, 31));
        no_move_spinBox->setMaximum(100);
        no_move_spinBox->setValue(10);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 81, 41));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 70, 81, 41));
        loitering_move_spinBox = new QSpinBox(groupBox);
        loitering_move_spinBox->setObjectName(QStringLiteral("loitering_move_spinBox"));
        loitering_move_spinBox->setGeometry(QRect(100, 80, 61, 31));
        loitering_move_spinBox->setMaximum(500);
        loitering_move_spinBox->setValue(100);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 81, 41));
        direction_spinBox = new QSpinBox(groupBox);
        direction_spinBox->setObjectName(QStringLiteral("direction_spinBox"));
        direction_spinBox->setGeometry(QRect(100, 120, 61, 31));
        direction_spinBox->setMaximum(360);
        direction_spinBox->setValue(30);
        buttonBox = new QDialogButtonBox(ObjectTrackingForm);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(40, 200, 161, 41));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(ObjectTrackingForm);

        QMetaObject::connectSlotsByName(ObjectTrackingForm);
    } // setupUi

    void retranslateUi(QWidget *ObjectTrackingForm)
    {
        ObjectTrackingForm->setWindowTitle(QApplication::translate("ObjectTrackingForm", "Form", 0));
        groupBox->setTitle(QApplication::translate("ObjectTrackingForm", "Object Tracking", 0));
        label->setText(QApplication::translate("ObjectTrackingForm", "No Move", 0));
        label_2->setText(QApplication::translate("ObjectTrackingForm", "Loitering", 0));
        label_3->setText(QApplication::translate("ObjectTrackingForm", "Direction", 0));
    } // retranslateUi

};

namespace Ui {
    class ObjectTrackingForm: public Ui_ObjectTrackingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTTRACKINGFORM_H
