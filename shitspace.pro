#-------------------------------------------------
#
# Project created by QtCreator 2014-09-20T14:55:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shitspace
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -lX11 -L/usr/include/X11
