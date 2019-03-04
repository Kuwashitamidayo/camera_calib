#include "calibgui.h"
#include "ui_calibgui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>




CalibGui::CalibGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalibGui)
{
    ui->setupUi(this);
    ui->textImagesPath->setText("No files selected");
    ui->textCameraParamsPath->setText("No files selected");
}

/*
void CalibGui::loadFromFile()
{

}
*/


CalibGui::~CalibGui()
{
    delete ui;
}


//QImage a ((uchar*) someMat.data, someMat.cols, someMat.rows, someMat.step, QImage::Format_RGB888);
//QImage b ((uchar*) someMat.data, someMat.cols, someMat.rows, someMat.step, QImage);

//ui-> qtCalibImage ->setPixMap(QPixmap::fromImage(a);
//ui-> qtCalibImage ->setPixMap(QPixmap::fromImage(b);

void CalibGui::on_buttonPathImages_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        "Select images to calibrate the camera", QDir::currentPath(),
        "Image files (*.jpg *.jpeg *.png);; All Files (*)");
    ui->textImagesPath->setText(fileNames[0]);
}

void CalibGui::on_buttonPathCameraParam_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select camera configuration file", QDir::currentPath(),
        "Configuration files (*.xml *.yml);; All Files (*)");
    ui->textCameraParamsPath->setText(fileName);

}
