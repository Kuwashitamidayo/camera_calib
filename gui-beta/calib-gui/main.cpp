#include "calibgui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalibGui w;
    w.show();


    return a.exec();
}
