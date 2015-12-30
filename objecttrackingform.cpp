#include "objecttrackingform.h"
#include "ui_objecttrackingform.h"

ObjectTrackingForm::ObjectTrackingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectTrackingForm)
{
    ui->setupUi(this);
}

ObjectTrackingForm::~ObjectTrackingForm()
{
    delete ui;
}

void ObjectTrackingForm::requestObjectTrackingParameters()
{
    objectTrackingParameters params;
    params.thresholdNoMove = ui->no_move_spinBox->value();
    params.thresholdNoMove = ui->loitering_move_spinBox->value();
    params.thresholdDirection = ui->direction_spinBox->value();
    emit setObjectTrackingParameters(params);
}

void ObjectTrackingForm::on_buttonBox_accepted()
{
    objectTrackingParameters params;
    params.thresholdNoMove = ui->no_move_spinBox->value();
    params.thresholdNoMove = ui->loitering_move_spinBox->value();
    params.thresholdDirection = ui->direction_spinBox->value();
    emit setObjectTrackingParameters(params);

    this->close();
}

void ObjectTrackingForm::on_buttonBox_rejected()
{
    this->close();
}
