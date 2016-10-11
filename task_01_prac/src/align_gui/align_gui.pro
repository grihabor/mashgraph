#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T17:31:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = align_gui
TEMPLATE = app


SOURCES += \
    sources/imageholder.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/pluginscontainer.cpp \
    ../shared/sources/crop.cpp \
    ../shared/sources/model.cpp \
    ../shared/sources/pluginmanager.cpp

HEADERS  += \
    headers/imageholder.h \
    headers/mainwindow.h \
    headers/ui_mainwindow.h \
    headers/pluginscontainer.h \
    ../shared/headers/crop.h \
    ../shared/headers/matrix.h \
    ../shared/headers/matrix.hpp \
    ../shared/headers/model.h \
    ../shared/headers/pluginfilter.h \
    ../shared/headers/pluginmanager.h \
    ../shared/headers/logger.h

LIBS += -ldl
QMAKE_CXXFLAGS += -std=c++11

FORMS    += \
    forms/mainwindow.ui
