#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QDockWidget>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QDebug>
#include <ocvcalib.h>

namespace Ui {
  class CameraSettings;
}

class CameraSettings : public QDockWidget
{
  Q_OBJECT

public:
  explicit CameraSettings(QWidget *parent = 0);
  virtual void setCameraSettings(CalibParams camera);
  virtual CalibParams getCalibParams();
  virtual bool isCalibParamsReady();

  ~CameraSettings();

private slots:
  void on_textCamPixSizeX_textChanged(const QString &arg1);
  void on_textCamPixSizeY_textChanged(const QString &arg1);
  void on_textMaxCamResX_textChanged(const QString &arg1);
  void on_textMaxCamResY_textChanged(const QString &arg1);
  void on_butApply_clicked();
  void on_textFocalLength_textChanged(const QString &arg1);
  void on_textCurrentCamResX_textChanged(const QString &arg1);
  void on_textCurrentCamResY_textChanged(const QString &arg1);

private:
  Ui::CameraSettings *ui;
  CalibParams camera;
  int camCurrResX, camCurrResY, camMaxResX, camMaxResY;
  double camMatrixSizeX, camMatrixSizeY, camPixSizeX, camPixSizeY;
  double focalLength;
  bool pixXEnable, pixYEnable, currResXEnable,
  currResYEnable, maxResXEnable, maxResYEnable, focalEnable;


};

#endif // CAMERASETTINGS_H
