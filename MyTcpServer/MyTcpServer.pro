QT += core network sql

CONFIG += console
CONFIG += c++17

SOURCES += main.cpp \
           database.cpp \
           mytcpserver.cpp \
           utils.cpp

HEADERS += mytcpserver.h \
    database.h \
    utils.h
