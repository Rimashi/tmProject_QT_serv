QT += core gui widgets network
CONFIG += c++17

TARGET = AuthClient
TEMPLATE = app

INCLUDEPATH += C:/msys64/mingw64/include
LIBS += -LC:/msys64/mingw64/lib -lgmp -lgmpxx

SOURCES += \
    adminstatisticsscreen.cpp \
    adminusersscreen.cpp \
    aes.cpp \
    client_api.cpp \
    func2client.cpp \
    main.cpp \
    mainwindow.cpp \
    authwidget.cpp \
    rsa.cpp \
    sha384.cpp \
    statisticsdialog.cpp \
    studentmainscreen.cpp \
    test1dialog.cpp \
    test2dialog.cpp \
    userstatisticsdialog.cpp \
    windowmanager.cpp

HEADERS += \
    adminstatisticsscreen.h \
    adminusersscreen.h \
    aes.h \
    client_api.h \
    func2client.h \
    mainwindow.h \
    authwidget.h \
    rsa.h \
    sha384.h \
    statisticsdialog.h \
    studentmainscreen.h \
    test1dialog.h \
    test2dialog.h \
    userstatisticsdialog.h \
    windowmanager.h
