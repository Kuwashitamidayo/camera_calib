#include "calibgui.h"
#include "ui_calibgui.h"

using namespace cv;
using namespace std;

QTime fileTime;
QStringList fileNames;          //for images
QString fileName;               //for config file
vector<cv::String> cvFiles;     //for image processing
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
      ui->labelIsChessFound->
              setStyleSheet("QLabel {background-color : black; color: red;}");
      QString path = "../../Pics/calib_pic_000.png";        //for tests only
      cv::Mat inputImage = cv::imread(toCvString(path), 1); //for tests only
      updatePicture(inputImage);                            //for tests only

}

/* Returns the current time with space (format: "HH:MM ") */
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

void CalibGui::updatePicture(cv::Mat someMat) {
    QImage receivedImage ((uchar*) someMat.data, someMat.cols, someMat.rows, someMat.step, QImage::Format_RGB888);
    ui->imgCalibImage->setScaledContents(true);
    ui->imgCalibImage->setPixmap(QPixmap::fromImage(receivedImage));
}

void CalibGui::updatePicture(vector<cv::Mat> someMat, int count) {
    QImage receivedImage ((uchar*) someMat[count-1].data, someMat[count-1].cols,
            someMat[count-1].rows, someMat[count-1].step, QImage::Format_RGB888);
    ui->imgCalibImage->setScaledContents(true);
    ui->imgCalibImage->setPixmap(QPixmap::fromImage(receivedImage));

    ui->textImagesCount->setText(QString::number(count) + " / "
                                 + QString::number(someMat.size()));
    if (found[count-1]) {
        ui->labelIsChessFound->setText("Chessboard found");
        ui->labelIsChessFound->setStyleSheet("QLabel {background-color : black;"
                                             " color: green;}");
    } else {
        ui->labelIsChessFound->setText("Chessboard not found");
        ui->labelIsChessFound->setStyleSheet("QLabel {background-color : black;"
                                             " color: red;}");
    }
}

cv::String CalibGui::toCvString(QString text) {
    return (cv::String) text.toStdString();
}

vector<cv::String> CalibGui::toVector(QStringList texts) {
    vector<cv::String> cvList;
    for (int i = 0; i < texts.size(); i++) {
        cvList.push_back(toCvString(texts[i]));
    }
    return cvList;
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

    vector<Vec2f> foundPoints;
    chessboardDimensions = cv::Size(camera.chessboardWidth - 1,
                                    camera.chessboardHeight - 1);

    fileNames = QFileDialog::getOpenFileNames(this,
        "Select images to calibrate the camera", QDir::currentPath(),
        "Image files (*.jpg *.jpeg *.png);; All Files (*)");
    if(fileNames.size() > 0) {
        // clearing vectors
        found.clear();
        cvFiles.clear();
        matChessPics.clear();

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
            cvFiles.push_back(toCvString(fileNames[i]));

            cv::Mat tempPic = imread(cvFiles[i]);
            found.push_back(findChessboardCorners(tempPic, chessboardDimensions,
                                          foundPoints,
                                          CV_CALIB_CB_ADAPTIVE_THRESH |
                                          CV_CALIB_CB_NORMALIZE_IMAGE));
            drawChessboardCorners(tempPic, chessboardDimensions,
                                  foundPoints, found[i]);
            matChessPics.push_back(tempPic);
        }
        updatePicture(matChessPics, 1);

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
            ::loadParametersFromXml(toCvString(fileName), camera);
            ui->textCalibBoardW->setText(QString::number(camera.chessboardWidth));
            ui->textCalibBoardH->setText(QString::number(camera.chessboardHeight));
            ui->textCalibSquareSize->setText(QString::number(camera.calibrationSquareSize));
            cameraSettings.setCameraSettings(camera);
      }
}

void CalibGui::on_buttonChangeCamSettings_clicked()
{
            cameraSettings.show();
}

void CalibGui::on_pButPrevPicture_clicked()
{
    if (countImages > 1) {
        countImages--;
        updatePicture(matChessPics, countImages);
    }
}

void CalibGui::on_pButNextPicture_clicked()
{
    if (countImages < totalImages) {
        countImages++;
        updatePicture(matChessPics, countImages);
    }
}
