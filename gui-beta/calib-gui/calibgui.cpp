#include "calibgui.h"
#include "ui_calibgui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTime>


QTime fileTime;
QStringList fileNames;    //for images
QString fileName;         //for config file


CalibGui::CalibGui(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CalibGui)
{
  ui->setupUi(this);
  ui->textImagesPath->setText("No files selected");
  ui->textCameraParamsPath->setText("No files selected");
}

/** Returns the current time with space (format: "HH:MM ") **/
QString CalibGui::getLogTime() {
  return fileTime.currentTime().toString("HH:mm") + " ";
}


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
  int count;
  fileNames = QFileDialog::getOpenFileNames(this,
    "Select images to calibrate the camera", QDir::currentPath(),
    "Image files (*.jpg *.jpeg *.png);; All Files (*)");
  if(fileNames.size() > 0) {
    count = fileNames.size()-1;
    if (count == 0) {
        ui->textImagesPath->setText(fileNames[0]);
    } else {
        ui->textImagesPath->setText(fileNames[0] + " and " +
          QString::number(count) + " more");
    }
      for (int i = 0; i <= count; i++) {
        ui->textLogWindow->append(getLogTime() + "Added " + fileNames[i]);
      }
      ui->textLogWindow->append(getLogTime() + "Total: " +
        QString::number(fileNames.size()) + " images.");

  }
}

void CalibGui::on_buttonPathCameraParam_clicked()
{
  fileName = QFileDialog::getOpenFileName(this,
    "Select camera configuration file", QDir::currentPath(),
    "Configuration files (*.xml *.yml);; All Files (*)");
  if(fileName != "") {
    ui->textCameraParamsPath->setText(fileName);
    ui->textLogWindow->append(getLogTime() + "Loaded " + fileName);
  }
}
