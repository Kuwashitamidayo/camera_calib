#ifndef CALIBGUI_H
#define CALIBGUI_H

#include <QMainWindow>
#include <QTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QDebug>
#include <camerasettings.h>
#include <opencv2/opencv.hpp>
#include <ocvcalib.h>

namespace Ui {
  class CalibGui;
}



class CalibGui : public QMainWindow
{
    Q_OBJECT

public:

    explicit CalibGui(QWidget *parent = 0);
    virtual QString getLogTime();
    virtual void updatePicture(int count);
    virtual void updatePicture(cv::Mat someMat);
    virtual void updatePicture(vector<cv::Mat> someMat, int count);
    virtual cv::String toCvString(QString text);
    virtual std::vector<cv::String> toVector(QStringList texts);
    virtual std::vector<cv::Mat> extractPicsWithChessboard(vector<cv::Mat> images);
    virtual void displayCameraMatrix(int row, double value_col1, double value_col2, double value_col3);
    virtual void displayCameraMatrix(int row, QString value_col1, QString value_col2, QString value_col3);
    virtual void displayCameraMatrix(CalibParams camera);
    virtual void displayDistanceCoefficients(cv::Mat distanceCoefficients);
    void changeEvent(QEvent * e);
    ~CalibGui();

private slots:
    void on_buttonPathImages_clicked();
    void on_buttonPathCameraParam_clicked();
    void on_buttonChangeCamSettings_clicked();
    void on_pButPrevPicture_clicked();
    void on_pButNextPicture_clicked();
    void on_buttonExportCameraParam_clicked();
    void on_pButStartCalibration_clicked();

private:
    Ui::CalibGui *ui;
    CameraSettings cameraSettings;
    cv::CalibParams camera;
    cv::Size chessboardDimensions;
    vector<cv::Mat> matChessPics;
    vector<bool> found;
    cv::Mat cameraMatrix;

    int minAmountOfPicsToCalibrate;
    int countImages;
    int totalImages;

};



#endif // CALIBGUI_H
