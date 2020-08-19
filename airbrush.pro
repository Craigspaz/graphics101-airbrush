QT += core gui widgets

TARGET = airbrush
CONFIG -= app_bundle

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    canvaswidget.cpp \
    airbrush.cpp \
    image.cpp

HEADERS += \
    mainwindow.h \
    canvaswidget.h \
    airbrush.h \
    image.h

FORMS += \
    mainwindow.ui
