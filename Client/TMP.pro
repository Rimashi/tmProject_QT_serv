QT += core gui widgets network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminstatwindow.cpp \
    adminwindow.cpp \
    authwindow.cpp \
    clientapi.cpp \
    func2client.cpp \
    global.cpp \
    main.cpp \
    mainwindow.cpp \
    networkclient.cpp \
    registerwindow.cpp \
    regwindow.cpp \
    statisticwindow.cpp \
    testwindow1.cpp \
    testwindow2.cpp \
    testwindow3.cpp \
    testwindow4.cpp \
    viewwindow.cpp

HEADERS += \
    adminstatwindow.h \
    adminwindow.h \
    authwindow.h \
    clientapi.h \
    func2client.h \
    global.h \
    mainwindow.h \
    networkclient.h \
    registerwindow.h \
    regwindow.h \
    statisticwindow.h \
    testwindow1.h \
    testwindow2.h \
    testwindow3.h \
    testwindow4.h \
    viewwindow.h

FORMS += \
    adminstatwindow.ui \
    adminwindow.ui \
    authwindow.ui \
    clientapi.ui \
    mainwindow.ui \
    networkclient.ui \
    registerwindow.ui \
    statisticwindow.ui \
    testwindow1.ui \
    testwindow2.ui \
    testwindow3.ui \
    testwindow4.ui \
    viewwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourse.qrc
