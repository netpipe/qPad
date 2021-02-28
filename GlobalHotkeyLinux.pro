#-------------------------------------------------
#
# Project created by QtCreator 2016-02-12T23:41:57
#
#-------------------------------------------------

QT       += core gui
QT       += x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG    += c++11
CONFIG    += link_pkgconfig
PKGCONFIG += x11

TARGET = GlobalHotkeyLinux
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nativeeventfilter.cpp

HEADERS  += mainwindow.h \
    nativeeventfilter.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
