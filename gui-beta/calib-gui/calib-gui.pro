#-------------------------------------------------
#
# Project created by QtCreator 2019-03-03T17:45:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calib-gui
TEMPLATE = app

INCLUDEPATH += -I. -I/usr/local/include/opencv-lopencv_features2d
LIBS += -L/usr/local/lib/opencv -lopencv_core -lopencv_imgproc \
-lopencv_imgcodecs -lopencv_features2d -lopencv_video -lopencv_ccalib \

SOURCES += main.cpp\
        calibgui.cpp \
    camerasettings.cpp

HEADERS  += calibgui.h \
    camerasettings.h

FORMS    += calibgui.ui \
    camerasettings.ui
