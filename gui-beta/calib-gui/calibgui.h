#ifndef CALIBGUI_H
#define CALIBGUI_H

#include <QMainWindow>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QDebug>
#include <camerasettings.h>
#include <opencv2/opencv.hpp>

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
    virtual cv::String toCvString(QString text);
    virtual std::vector<cv::String> toVector(QStringList texts);
    ~CalibGui();

private slots:
    void on_buttonPathImages_clicked();

    void on_buttonPathCameraParam_clicked();

    void on_buttonChangeCamSettings_clicked();

    void on_pButPrevPicture_clicked();

    void on_pButNextPicture_clicked();

private:
    Ui::CalibGui *ui;
    CameraSettings window;

};



#endif // CALIBGUI_H
