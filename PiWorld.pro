#-------------------------------------------------
#
# Project created by QtCreator 2017-11-10T08:48:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotorTester
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
            mainwindow.cpp \
            commpi.cpp \
            motor.cpp

HEADERS  += mainwindow.h \
            commpi.h \
            motor.h

FORMS    += mainwindow.ui

LIBS    = -L/usr/lib -lwiringPi
