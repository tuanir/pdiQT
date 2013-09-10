#-------------------------------------------------
#
# Project created by QtCreator 2013-09-05T21:06:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

LIBS += `pkg-config opencv --cflags --libs`

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
