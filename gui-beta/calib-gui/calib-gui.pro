#-------------------------------------------------
#
# Project created by QtCreator 2019-03-03T17:45:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calib-gui
TEMPLATE = app


SOURCES += main.cpp\
        calibgui.cpp \
    camerasettings.cpp \
    ocvcalib.cpp

HEADERS  += calibgui.h \
    camerasettings.h

FORMS    += calibgui.ui \
    camerasettings.ui
