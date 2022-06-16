#-------------------------------------------------
#
# Project created by QtCreator 2022-04-05T14:50:12
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = comPortGenerate
TEMPLATE = app


SOURCES += main.cpp\
        generatedata.cpp

HEADERS  += generatedata.h

FORMS    += generatedata.ui

RESOURCES +=
RC_FILE = server.rc
