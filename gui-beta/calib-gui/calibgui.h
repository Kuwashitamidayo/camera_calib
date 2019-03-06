#ifndef CALIBGUI_H
#define CALIBGUI_H

#include <QMainWindow>
#include <camerasettings.h>

namespace Ui {
  class CalibGui;
}

class CalibGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibGui(QWidget *parent = 0);
    virtual QString getLogTime();
    virtual void showCameraSettings();
    ~CalibGui();

private slots:
    void on_buttonPathImages_clicked();

    void on_buttonPathCameraParam_clicked();

    void on_buttonChangeCamSettings_clicked();

private:
    Ui::CalibGui *ui;
    CameraSettings window;
};



#endif // CALIBGUI_H
