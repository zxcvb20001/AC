#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T23:27:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = SubMachine
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    netcontroller.cpp

HEADERS  += mainwindow.h \
    netcontroller.h \
    constants.h

FORMS    += mainwindow.ui
