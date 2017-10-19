#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T14:44:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = search
TEMPLATE = app


SOURCES += main.cpp\
    FileSearchThread.cpp \
    SearchUI.cpp

HEADERS  += FileSearchThread.h \
    SearchUI.h

FORMS    += \
    Search.ui


