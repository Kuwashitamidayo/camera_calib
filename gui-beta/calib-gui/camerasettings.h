#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QDockWidget>

namespace Ui {
  class CameraSettings;
}

class CameraSettings : public QDockWidget
{
  Q_OBJECT

public:
  explicit CameraSettings(QWidget *parent = 0);
  ~CameraSettings();

private slots:
  void on_textCamPixSizeX_textChanged(const QString &arg1);

  void on_textCamPixSizeY_textChanged(const QString &arg1);

  void on_textMaxCamResX_textChanged(const QString &arg1);

  void on_textMaxCamResY_textChanged(const QString &arg1);

private:
  Ui::CameraSettings *ui;
  int camCurrResX, camCurrResY, camMaxResX, camMaxResY;
  double camMatrixSizeX, camMatrixSizeY, camPixSizeX, camPixSizeY;
  double focalLength;


};

#endif // CAMERASETTINGS_H
