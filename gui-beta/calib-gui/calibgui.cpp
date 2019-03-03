#include "calibgui.h"
#include "ui_calibgui.h"

CalibGui::CalibGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalibGui)
{
    ui->setupUi(this);
}

CalibGui::~CalibGui()
{
    delete ui;
}


//QImage a ((uchar*) someMat.data, someMat.cols, someMat.rows, someMat.step, QImage::Format_RGB888);
//QImage b ((uchar*) someMat.data, someMat.cols, someMat.rows, someMat.step, QImage);

//ui-> qtCalibImage ->setPixMap(QPixmap::fromImage(a);
//ui-> qtCalibImage ->setPixMap(QPixmap::fromImage(b);
