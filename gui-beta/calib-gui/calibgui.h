#ifndef CALIBGUI_H
#define CALIBGUI_H

#include <QMainWindow>

namespace Ui {
class CalibGui;
}

class CalibGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibGui(QWidget *parent = 0);
    ~CalibGui();

private:
    Ui::CalibGui *ui;
};

#endif // CALIBGUI_H
