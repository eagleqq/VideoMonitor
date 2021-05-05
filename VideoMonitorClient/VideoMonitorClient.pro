#-------------------------------------------------
#
# Project created by QtCreator 2021-04-27T13:45:25
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoMonitorClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginsystemdialog.cpp \
    iconbutton.cpp \
    onevideo.cpp \
    recvthread.cpp \
    configdialog.cpp \
    util.cpp \
    sqlitesingleton.cpp

HEADERS  += mainwindow.h \
    loginsystemdialog.h \
    iconbutton.h \
    onevideo.h \
    recvthread.h \
    configdialog.h \
    util.h \
    sqlitesingleton.h

FORMS    += mainwindow.ui \
    loginsystemdialog.ui \
    configdialog.ui

INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
              /usr/local/include/opencv2 \

LIBS += /usr/local/lib/libopencv_*.so \

RESOURCES += \
    img.qrc
