#include "camerasettings.h"
#include "ui_camerasettings.h"

CameraSettings::CameraSettings(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CameraSettings)
{
    QDoubleValidator *camElemSizeVal = new QDoubleValidator( 0.000001, 1000.0, 6, this );
    camElemSizeVal->setNotation(QDoubleValidator::StandardNotation);
    QIntValidator *resVal = new QIntValidator( 1, 10000, this );
    ui->setupUi(this);
    ui->textCamMatrixSizeX->setValidator(camElemSizeVal);
    ui->textCamMatrixSizeY->setValidator(camElemSizeVal);
    ui->textCamPixSizeX->setValidator(camElemSizeVal);
    ui->textCamPixSizeY->setValidator(camElemSizeVal);
    ui->textFocalLength->setValidator(camElemSizeVal);
    ui->textMaxCamResX->setValidator(resVal);
    ui->textMaxCamResY->setValidator(resVal);
    ui->textCurrentCamResX->setValidator(resVal);
    ui->textCurrentCamResY->setValidator(resVal);
    camCurrResX = 0;
    camCurrResY = 0;
    camMaxResX = 0;
    camMaxResY = 0;
    camMatrixSizeX = 0.0;
    camMatrixSizeY = 0.0;
    camPixSizeX = 0.0;
    camPixSizeY = 0.0;
    focalLength = 0.0;
}

CameraSettings::~CameraSettings()
{
    delete ui;
}

void CameraSettings::on_textCamPixSizeX_textChanged(const QString &arg1)
{
    this->camPixSizeX = arg1.toDouble();
    this->camMatrixSizeX = camPixSizeX * camMaxResX;
    ui->textCamMatrixSizeX->setText(QString::number(camMatrixSizeX));
}

void CameraSettings::on_textCamPixSizeY_textChanged(const QString &arg1)
{
    this->camPixSizeY = arg1.toDouble();
    this->camMatrixSizeY = camPixSizeY * camMaxResY;
    ui->textCamMatrixSizeY->setText(QString::number(camMatrixSizeY));
}

void CameraSettings::on_textMaxCamResX_textChanged(const QString &arg1)
{
    this->camMaxResX = arg1.toDouble();
    this->camMatrixSizeX = camPixSizeX * camMaxResX;
    ui->textCamMatrixSizeX->setText(QString::number(camMatrixSizeX));
}

void CameraSettings::on_textMaxCamResY_textChanged(const QString &arg1)
{
    this->camMaxResY = arg1.toDouble();
    this->camMatrixSizeY = camPixSizeY * camMaxResY;
    ui->textCamMatrixSizeY->setText(QString::number(camMatrixSizeY));
}
