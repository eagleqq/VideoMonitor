#-------------------------------------------------
#
# Project created by QtCreator 2021-05-04T13:52:39
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoMonitorServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    v4l2api.cpp

HEADERS  += mainwindow.h \
    v4l2api.h

FORMS    += mainwindow.ui

INCLUDEPATH += ./libjpeg
LIBS += -L./libjpeg -ljpeg

