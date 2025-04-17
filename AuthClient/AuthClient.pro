QT += core gui widgets network
CONFIG += c++17

TARGET = AuthClient
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    authwidget.cpp \
    networkclient.cpp

HEADERS += \
    mainwindow.h \
    authwidget.h \
    networkclient.h
