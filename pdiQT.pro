#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T21:06:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

//INCLUDEPATH +=/usr/include/opencv
//INCLUDEPATH +=/usr/include/opencv2/
//INCLUDEPATH +=/usr/include/opencv2/core
//INCLUDEPATH +=/usr/include/opencv2/imgproc
//INCLUDEPATH +=/usr/include/opencv2/ml
//INCLUDEPATH +=/usr/include/opencv2/highgui
//INCLUDEPATH +=/usr/bin/ld
//LIBS +=-L/usr/bin
//LIBS += -L/usr/lib/libopencv_contrib.so
//LIBS += -LD: /usr/lib
LIBS += `pkg-config opencv --cflags --libs`

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
