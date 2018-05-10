#-------------------------------------------------
#
# Project created by QtCreator 2017-02-10T09:37:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = STM32BL_FlashTool
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    masterthread.cpp

HEADERS  += mainwindow.h \
    masterthread.h

FORMS    += mainwindow.ui
