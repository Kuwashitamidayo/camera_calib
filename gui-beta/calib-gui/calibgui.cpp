#include "calibgui.h"
#include "ui_calibgui.h"
//#define CALIB_PRINT_DEBUG 1

using namespace cv;
using namespace std;

QTime fileTime;
QStringList fileNames;          //for images
QString fileName;               //for config file
vector<cv::String> cvFiles;     //for image processing
vector<cv::Mat> cvImages;



CalibGui::CalibGui(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::CalibGui),
      minAmountOfPicsToCalibrate(5),    //minimal amount of pics needed to allow to calibrate
      countImages(0),   // number of picture that is shown
      totalImages(0)    // total amount of pictures selected for calibration
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

      ui->gridCamMatrix->addWidget(ui->labelCameraMatrix, 0, 0, 1, 3);
      ui->labelCameraMatrix->setText("Camera Matrix:");

      displayCameraMatrix(1, "fx",  "0",    "cx");
      displayCameraMatrix(2, "0",   "fy",   "cy");
      displayCameraMatrix(3, "0",   "0",    "1" );
      ui->gridDistCoeff->addWidget(ui->labelDistanceCoefficients, 0, 0, 1, 2);
      ui->labelDistanceCoefficients->setText("Distance Coefficients:");
      ui->gridDistCoeff->addWidget(ui->labelDistCoeff5, 3, 0, 1, 2);
//      QString path = "../../Pics/calib_pic_000.png";        //for tests only
//      cv::Mat inputImage = cv::imread(toCvString(path), 1); //for tests only
//      updatePicture(inputImage);                            //for tests only

}

CalibGui::~CalibGui()
{
    delete ui;
}

/* Returns the current time with space (format: "HH:MM ") */
QString CalibGui::getLogTime() {
      return fileTime.currentTime().toString("HH:mm") + " ";
}

// Displays count'th picture if loaded,
void CalibGui::updatePicture(int count) {
    QPixmap imageCalib;
    imageCalib.load(fileNames[count-1]);
    ui->imgCalibImage->setScaledContents(true);
    ui->imgCalibImage->setPixmap(imageCalib);
    ui->textImagesCount->setText(QString::number(count) + " / "
                                 + QString::number(totalImages));
}

// Displays picture given from OpenCV Mat class
void CalibGui::updatePicture(cv::Mat someMat) {
    QImage receivedImage ((uchar*) someMat.data, someMat.cols, someMat.rows, someMat.step, QImage::Format_RGB888);
    ui->imgCalibImage->setScaledContents(true);
    ui->imgCalibImage->setPixmap(QPixmap::fromImage(receivedImage));
}

// Displays count'th picture from vector of Mats
void CalibGui::updatePicture(vector<cv::Mat> someMat, int count) {
    QImage receivedImage ((uchar*) someMat[count-1].data, someMat[count-1].cols,
            someMat[count-1].rows, someMat[count-1].step, QImage::Format_RGB888);

    ui->imgCalibImage->setScaledContents(true);
    ui->imgCalibImage->setPixmap(QPixmap::fromImage(receivedImage));
    ui->textImagesCount->setText(QString::number(count) + " / "
                                 + QString::number(someMat.size()));

    if (!found[count-1]) {
        ui->labelIsChessFound->setText("Chessboard not found");
        ui->labelIsChessFound->setStyleSheet("QLabel {background-color : black;"
                                             " color: red;}");
        return;
    }

    ui->labelIsChessFound->setText("Chessboard found");
    ui->labelIsChessFound->setStyleSheet("QLabel {background-color : black;"
                                         " color: green;}");
}

// Converts QString to cv::String
cv::String CalibGui::toCvString(QString text) {
    return (cv::String) text.toStdString();
}

// Converts QStringList to vector of cv::Strings
vector<cv::String> CalibGui::toVector(QStringList texts) {
    vector<cv::String> cvList;
    for (int i = 0; i < texts.size(); i++) {
        cvList.push_back(toCvString(texts[i]));
    }
    return cvList;
}

vector<cv::Mat> CalibGui::extractPicsWithChessboard(vector<cv::Mat> images) {
    vector<cv::Mat> resultImages;
    for (unsigned int i = 0; i < images.size(); i++) {
        if (this->found[i]) resultImages.push_back(images[i]);
    }
    return resultImages;
}

// Writes rows of the camera matrix to display it on GUI
void CalibGui::displayCameraMatrix(int row, double value_col1, double value_col2, double value_col3) {
    switch (row)
    {
    case 1:
        ui->labelCamRow1_1->setText(QString::number(value_col1));
        ui->labelCamRow1_2->setText(QString::number(value_col2));
        ui->labelCamRow1_3->setText(QString::number(value_col3));
        break;
    case 2:
        ui->labelCamRow2_1->setText(QString::number(value_col1));
        ui->labelCamRow2_2->setText(QString::number(value_col2));
        ui->labelCamRow2_3->setText(QString::number(value_col3));
        break;
    case 3:
        ui->labelCamRow3_1->setText(QString::number(value_col1));
        ui->labelCamRow3_2->setText(QString::number(value_col2));
        ui->labelCamRow3_3->setText(QString::number(value_col3));
        break;
    }
}

// Writes rows of the camera matrix to display it on GUI
void CalibGui::displayCameraMatrix(int row, QString value_col1, QString value_col2, QString value_col3) {
    switch (row)
    {
    case 1:
        ui->labelCamRow1_1->setText(value_col1);
        ui->labelCamRow1_2->setText(value_col2);
        ui->labelCamRow1_3->setText(value_col3);
        break;
    case 2:
        ui->labelCamRow2_1->setText(value_col1);
        ui->labelCamRow2_2->setText(value_col2);
        ui->labelCamRow2_3->setText(value_col3);
        break;
    case 3:
        ui->labelCamRow3_1->setText(value_col1);
        ui->labelCamRow3_2->setText(value_col2);
        ui->labelCamRow3_3->setText(value_col3);
        break;
    }
}

void CalibGui::displayCameraMatrix(CalibParams camera) {
    cv::Mat cameraMatrix = ::getCameraMatrix(camera);
    ui->labelCamRow1_1->setText(QString::number(cameraMatrix.at<double>(0, 0)));
    ui->labelCamRow1_2->setText(QString::number(cameraMatrix.at<double>(0, 1)));
    ui->labelCamRow1_3->setText(QString::number(cameraMatrix.at<double>(0, 2)));
    ui->labelCamRow2_1->setText(QString::number(cameraMatrix.at<double>(1, 0)));
    ui->labelCamRow2_2->setText(QString::number(cameraMatrix.at<double>(1, 1)));
    ui->labelCamRow2_3->setText(QString::number(cameraMatrix.at<double>(1, 2)));
    ui->labelCamRow3_1->setText(QString::number(cameraMatrix.at<double>(2, 0)));
    ui->labelCamRow3_2->setText(QString::number(cameraMatrix.at<double>(2, 1)));
    ui->labelCamRow3_3->setText(QString::number(cameraMatrix.at<double>(2, 2)));
}

void CalibGui::displayDistanceCoefficients(cv::Mat distanceCoefficients) {
    ui->labelDistCoeff1->setText(QString::number(distanceCoefficients.at<double>(0, 0)));
    ui->labelDistCoeff2->setText(QString::number(distanceCoefficients.at<double>(1, 0)));
    ui->labelDistCoeff3->setText(QString::number(distanceCoefficients.at<double>(2, 0)));
    ui->labelDistCoeff4->setText(QString::number(distanceCoefficients.at<double>(3, 0)));
    ui->labelDistCoeff5->setText(QString::number(distanceCoefficients.at<double>(4, 0)));
}

void CalibGui::changeEvent(QEvent * e) {
    if(e->type() == QEvent::ActivationChange && this->isActiveWindow()) {
        if (cameraSettings.isCalibParamsReady())
            displayCameraMatrix(cameraSettings.getCalibParams());
//        ui->textLogWindow->append(getLogTime() + "Changed activated window.");
    }
}


/*
 * Buttons and other GUI based actions are served below
 */

// Load pictures, providing that parameters are given
void CalibGui::on_buttonPathImages_clicked()
{
    int count;
    QString msg;

    vector<Vec2f> foundPoints;
    try {
        camera.chessboardWidth = ui->textCalibBoardW->text().toInt();
        camera.chessboardHeight = ui->textCalibBoardH->text().toInt();
        camera.calibrationSquareSize = ui->textCalibSquareSize->text().toDouble();
    } catch (Exception& e) {
        camera.chessboardWidth = 0;
        camera.chessboardHeight = 0;
        camera.calibrationSquareSize = 0.0;
    }


    /* in window you enter number of squares in x and y, you need intersections
     * between squares, that's why 1 s substracted */
    chessboardDimensions = cv::Size(camera.chessboardWidth - 1,
                                    camera.chessboardHeight - 1);

    #ifdef CALIB_PRINT_DEBUG
    msg = "Chessboard width: " + QString::number(camera.chessboardWidth) + "mm\n"
                  "Chessboard height: " + QString::number(camera.chessboardHeight) + "mm\n"
                  "Square side size: " + QString::number(camera.calibrationSquareSize) + "mm\n";
    QMessageBox::warning(this, "Checking the parameters...", msg);
    #endif

    if (((camera.chessboardHeight <= 3)) || (camera.chessboardWidth <= 3)
            || (camera.calibrationSquareSize <= 0))
    {
        // warning message if parameters are incorrect
        msg = "Please enter proper calibration chessboard settings first. "
                       "Otherwise, error will occur! \n"
                        "Chessboard must be min. 4x4.";
        QMessageBox::warning(this, "Warning - parameters are missing", msg);
        ui->textCalibBoardW->setFocus();
        return;
    }

    fileNames = QFileDialog::getOpenFileNames(this,
        "Select images to calibrate the camera", QDir::currentPath(),
        "Image files (*.jpg *.jpeg *.png);; All Files (*)");

    if(fileNames.size() <= 0) return;

    // clearing vectors
    found.clear();
    cvFiles.clear();
    cvImages.clear();
    matChessPics.clear();

    //setting 1st image as the one that should be displayed
    countImages = 1;
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
        cvImages.push_back(imread(cvFiles[i]));
        // that's why parameters are needed - chesboard is marked here
        cv::Mat tempPic = cvImages[i].clone();
        found.push_back(findChessboardCorners(tempPic, chessboardDimensions,
                                      foundPoints,
                                      CV_CALIB_CB_ADAPTIVE_THRESH |
                                      CV_CALIB_CB_NORMALIZE_IMAGE));
        drawChessboardCorners(tempPic, chessboardDimensions,
                              foundPoints, found[i]);
        matChessPics.push_back(tempPic);
    }
    updatePicture(matChessPics, countImages);

    ui->textLogWindow->append(getLogTime() + "Total: " +
        QString::number(fileNames.size()) + " images.");

    #ifdef CALIB_PRINT_DEBUG
    msg = "Count image: " + QString::number(countImages) + "\n"
                "Total images: " + QString::number(totalImages) + "\n";
    QMessageBox::warning(this, "Showing counted and total images...", msg);
    #endif

}

// Loads parameters from external xml/yml file
void CalibGui::on_buttonPathCameraParam_clicked()
{
      fileName = QFileDialog::getOpenFileName(this,
            "Select camera configuration file", QDir::currentPath(),
            "Configuration files (*.xml *.yml);; All Files (*)");

      if(fileName == "") return;

      try {
        ui->textCameraParamsPath->setText(fileName);
        ui->textLogWindow->append(getLogTime() + "Loaded " + fileName);
        ::loadParametersFromXml(toCvString(fileName), camera);

        if(camera.header == "" || camera.focalLength == 0 ||
                camera.pixelSize.x <= 0 || camera.pixelSize.y <= 0 ||
                camera.chessboardHeight <= 0 || camera.chessboardWidth <= 0) {
            throw std::invalid_argument("Received invalid or no arguments!");
        }

        ui->textCalibBoardW->setText(QString::number(camera.chessboardWidth));
        ui->textCalibBoardH->setText(QString::number(camera.chessboardHeight));
        ui->textCalibSquareSize->setText(QString::number(camera.calibrationSquareSize));

        cameraSettings.setCameraSettings(camera);
        displayCameraMatrix(camera);

      } catch (std::invalid_argument& ia) {
          QString msg = "Invalid content of the file - aborted. \n"
                        "Are you sure this is the config file with camera parameters? "
                        "If yes, please check if parameters are filled or OK. ";
          QMessageBox::warning(this, "Warning - wrong file (invalid parameters)", msg);
          ui->textLogWindow->append(getLogTime() + "Error while loading config file " + fileName
                                    + " - probably not a camera parameters file.");
      } catch (Exception& e) {
          QString msg = "Invalid content of the file - aborted. \n"
                        "Probably not a camera parameters config file.";
          QMessageBox::warning(this, "Warning - wrong file", msg);
          ui->textLogWindow->append(getLogTime() + "Error while loading config file " + fileName);
      }

}

// Shows window with camera paremeters
void CalibGui::on_buttonChangeCamSettings_clicked()
{
    cameraSettings.show();
}

// Show previous picture from vector
void CalibGui::on_pButPrevPicture_clicked()
{
    if (countImages > 1) {
        countImages--;
        updatePicture(matChessPics, countImages);
    }
}

// Show next picture from vector
void CalibGui::on_pButNextPicture_clicked()
{
    if (countImages < totalImages) {
        countImages++;
        updatePicture(matChessPics, countImages);
    }
}

// Export parameters to external xml/yml file
void CalibGui::on_buttonExportCameraParam_clicked()
{
    bool ok;
    QString fileType = "XML file (*.xml)";
    QString saveFile = ".xml";

    saveFile = QFileDialog::getSaveFileName(this,
        "Save parameters as configuration file", QDir::currentPath(),
        "XML file (*.xml);; YML file (*.yml);; All Files (*)", &fileType);

    QString text = QInputDialog::getText(0, "Input dialog",
                                         "Please enter the name of the camera:",
                                         QLineEdit::Normal,"", &ok);
    cv::String header = toCvString(text);

    if (ok)
        ::saveParametersToXml(toCvString(saveFile), this->camera, header);
}

// Starts the calibration, providing that parameters are given and pictures are loaded
void CalibGui::on_pButStartCalibration_clicked()
{
    Mat distanceCoefficients;
    vector<Mat> tVectors, rVectors;
    vector<Mat> savedImages = extractPicsWithChessboard(cvImages);
    cameraMatrix = ::getCameraMatrix(camera);
    chessboardDimensions = cv::Size(camera.chessboardWidth - 1,
                                    camera.chessboardHeight - 1);

    ::clearPerViewErrors(); //clear repr. errors

    if (savedImages.size() < (unsigned int)minAmountOfPicsToCalibrate)
    {
        QString msg = "Please load some pictures first! \n Remember, on at least "
                + QString::number((float)minAmountOfPicsToCalibrate) +
                " of them chessboard must be detected!";

        QMessageBox::warning(this, "Warning - no pictures loaded", msg);
        ui->textCalibBoardW->setFocus();
        ui->textLogWindow->append(getLogTime() +
                                  "Not enough pictures for calibration! At least "
                                  + (int)minAmountOfPicsToCalibrate +
                                  " of pictures are needed (loaded " +
                                  (int)savedImages.size() + " with chessboard detected).");
        return;
    }

    if (savedImages.size() < (unsigned int)minAmountOfPicsToCalibrate &&
            savedImages.size() > 0)
    {
        ui->textLogWindow->append(getLogTime() +
                                  "Not enough pictures for calibration! At least "
                                  + minAmountOfPicsToCalibrate +
                                  " of pictures are needed (loaded " +
                                  savedImages.size() + " with chessboard deected).");
        return;
    }

    // if everything is fine, start the calibration
    ui->textLogWindow->append(getLogTime() + "Calibration has started...");

    ::cameraCalibration(savedImages, chessboardDimensions,
      camera.calibrationSquareSize, cameraMatrix,
      distanceCoefficients, rVectors, tVectors, camera);

    ::saveCameraCalibration("camera_calibration.xml",
      "camera_calibration_pic_data.xml", cameraMatrix,
      distanceCoefficients, rVectors, tVectors);

    ui->textLogWindow->append(getLogTime() +
                              "Calibration ended successfully!");
    displayDistanceCoefficients(distanceCoefficients);
    ui->textLogWindow->append(getLogTime() +
                              "Reprojection error: " +
                              QString::number(::getReprojectionError()));

    vector<float> perViewErrors = ::getPerViewErrors();

    for (int i = 0; i < (int)perViewErrors.size(); i++)
    {
        ui->textLogWindow->append(
                    getLogTime() + "Image no. " + QString::number(i+1) +
                    " error: " + QString::number(perViewErrors[i]));
    }
}
