#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T12:44:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task_01_prac
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    model.cpp \
    crop.cpp \
    imageholder.cpp

HEADERS  += mainwindow.h \
    matrix.hpp \
    matrix.h \
    model.h \
    observer.h \
    crop.h \
    imageholder.h

FORMS    += mainwindow.ui
