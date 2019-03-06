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

private:
  Ui::CameraSettings *ui;
};

#endif // CAMERASETTINGS_H
