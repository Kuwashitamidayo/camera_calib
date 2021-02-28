#include "camerasettings.h"
#include "ui_camerasettings.h"

CameraSettings::CameraSettings(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CameraSettings),
    camCurrResX(0), camCurrResY(0),
    camMatrixSizeX(0.0), camMatrixSizeY(0.0),
    camPixSizeX(0.0), camPixSizeY(0.0), focalLength(0.0),
    pixXEnable(false), pixYEnable(false), currResXEnable(false),
    currResYEnable(false), maxResXEnable(false),
    maxResYEnable(false), focalEnable(false)
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
}

CameraSettings::~CameraSettings()
{
    delete ui;
}

void CameraSettings::setCameraSettings(CalibParams camera)
{
    ui->textCamMatrixSizeX  ->  setText(QString::number(camera.matrixSize.x));
    ui->textCamMatrixSizeY  ->  setText(QString::number(camera.matrixSize.y));
    ui->textCamPixSizeX     ->  setText(QString::number(camera.pixelSize.x));
    ui->textCamPixSizeY     ->  setText(QString::number(camera.pixelSize.y));
    ui->textFocalLength     ->  setText(QString::number(camera.focalLength));
    ui->textMaxCamResX      ->  setText(QString::number(camera.matrixMaxRes.x));
    ui->textMaxCamResY      ->  setText(QString::number(camera.matrixMaxRes.y));
    ui->textCurrentCamResX  ->  setText(QString::number(camera.matrixCurrRes.x));
    ui->textCurrentCamResY  ->  setText(QString::number(camera.matrixCurrRes.y));

    ui->comboCameraName     ->  addItem(QString::fromStdString((string)camera.header));
}

CalibParams CameraSettings::getCalibParams() {
    return this->camera;
}

bool CameraSettings::isCalibParamsReady() {
    return (pixXEnable && pixYEnable && maxResXEnable && maxResYEnable
            && focalEnable && currResXEnable && currResYEnable);
}


void CameraSettings::on_textCamPixSizeX_textChanged(const QString &arg1)
{
    this->camPixSizeX = arg1.toDouble();
    this->camMatrixSizeX = camPixSizeX * camMaxResX;
    ui->textCamMatrixSizeX->setText(QString::number(camMatrixSizeX));
    camera.pixelSize.x = arg1.toDouble();
    camera.matrixSize.x = this->camMatrixSizeX;
    pixXEnable = true;
}

void CameraSettings::on_textCamPixSizeY_textChanged(const QString &arg1)
{
    this->camPixSizeY = arg1.toDouble();
    this->camMatrixSizeY = camPixSizeY * camMaxResY;
    ui->textCamMatrixSizeY->setText(QString::number(camMatrixSizeY));
    camera.pixelSize.y = arg1.toDouble();
    camera.matrixSize.y = this->camMatrixSizeY;
    pixYEnable = true;
}

void CameraSettings::on_textMaxCamResX_textChanged(const QString &arg1)
{
    this->camMaxResX = arg1.toDouble();
    this->camMatrixSizeX = camPixSizeX * camMaxResX;
    ui->textCamMatrixSizeX->setText(QString::number(camMatrixSizeX));
    camera.matrixMaxRes.x = arg1.toInt();
    camera.matrixSize.x = this->camMatrixSizeX;
    maxResXEnable = true;
}

void CameraSettings::on_textMaxCamResY_textChanged(const QString &arg1)
{
    this->camMaxResY = arg1.toDouble();
    this->camMatrixSizeY = camPixSizeY * camMaxResY;
    ui->textCamMatrixSizeY->setText(QString::number(camMatrixSizeY));
    camera.matrixMaxRes.x = arg1.toInt();
    camera.matrixSize.y = this->camMatrixSizeY;
    maxResYEnable = true;
}

void CameraSettings::on_butApply_clicked()
{
    close();
}

void CameraSettings::on_textFocalLength_textChanged(const QString &arg1)
{
    camera.focalLength = arg1.toDouble();
    focalEnable = true;
}

void CameraSettings::on_textCurrentCamResX_textChanged(const QString &arg1)
{
    camera.matrixCurrRes.x = arg1.toDouble();
    currResXEnable = true;
}

void CameraSettings::on_textCurrentCamResY_textChanged(const QString &arg1)
{
    camera.matrixCurrRes.y = arg1.toDouble();
    currResYEnable = true;
}
