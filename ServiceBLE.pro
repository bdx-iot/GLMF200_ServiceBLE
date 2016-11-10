QT += core dbus bluetooth
QT -= gui

CONFIG += c++11

TARGET = ServiceBLE
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Service/CServiceRpmsgInterface.cpp \
    CServerBLE.cpp

HEADERS += \
    Service/CServiceRpmsgInterface.h \
    CServerBLE.h
