QT += core
QT += gui

CONFIG += c++11

TARGET = align_console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    sources/main.cpp \
    sources/controller.cpp \
    ../shared/sources/crop.cpp \
    ../shared/sources/model.cpp \
    ../shared/sources/pluginmanager.cpp

LIBS += -ldl
QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    headers/controller.h \
    ../shared/headers/crop.h \
    ../shared/headers/logger.h \
    ../shared/headers/matrix.h \
    ../shared/headers/matrix.hpp \
    ../shared/headers/model.h \
    ../shared/headers/pluginfilter.h \
    ../shared/headers/pluginmanager.h \
    headers/printer.h \
    headers/maintask.h
