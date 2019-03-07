#include "calibgui.h"
#include "ui_calibgui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QTime>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QDebug>


QTime fileTime;
QStringList fileNames;          //for images
QString fileName;               //for config file
int countImages = 0;
int totalImages = 0;




CalibGui::CalibGui(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::CalibGui)
{
      QDoubleValidator *squareSizeVal = new QDoubleValidator( 0.01, 100.0, 2, this );
      squareSizeVal->setNotation(QDoubleValidator::StandardNotation);
      QIntValidator *squareAmountVal = new QIntValidator( 1, 100, this );
      ui->setupUi(this);
      ui->textImagesPath->setText("No files selected");
      ui->textCameraParamsPath->setText("No files selected");
      ui->textCalibBoardH->setValidator(squareAmountVal);
      ui->textCalibBoardW->setValidator(squareAmountVal);
      ui->textCalibSquareSize->setValidator(squareSizeVal);
      qDebug() << "ui->textCalibSquareSize->validator()->parent()";
      qDebug() << ui->textCalibSquareSize->validator()->parent();
      ui->labelCamRow1->setText("fx\t\t\t\t0 \t\t\t\tcx");
      ui->labelCamRow2->setText("0 \t\t\t\tfy\t\t\t\tcy");
      ui->labelCamRow3->setText("0 \t\t\t\t0 \t\t\t\t1 ");
      //pLabel->setStyleSheet("QLabel { background-color : red; color : blue; }");
      ui->labelIsChessFound->
              setStyleSheet("QLabel {background-color : black; color: red;}");
}

/** Returns the current time with space (format: "HH:MM ") **/
QString CalibGui::getLogTime() {
      return fileTime.currentTime().toString("HH:mm") + " ";
}

void CalibGui::updatePicture(int count) {
    countImages = count;
    totalImages = fileNames.size();
    QPixmap imageCalib;
    imageCalib.load(fileNames[count-1]);
    ui->imgCalibImage->setScaledContents(true);
    ui->imgCalibImage->setPixmap(imageCalib);
    ui->textImagesCount->setText(QString::number(count) + " / "
                                 + QString::number(totalImages));
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
        totalImages = fileNames.size();
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
        updatePicture(1);

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

void CalibGui::on_buttonChangeCamSettings_clicked()
{
            window.show();
}

void CalibGui::on_pButPrevPicture_clicked()
{
    if (countImages > 1) {
        countImages--;
        updatePicture(countImages);
    }
}

void CalibGui::on_pButNextPicture_clicked()
{
    if (countImages < totalImages) {
        countImages++;
        updatePicture(countImages);
    }
}
