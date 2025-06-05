QT += core network sql

CONFIG += console
CONFIG += c++17

# GMP
INCLUDEPATH += C:/msys64/mingw64/include
LIBS += -LC:/msys64/mingw64/lib -lgmp -lgmpxx

# muParser
# INCLUDEPATH += $$PWD/muparser-master/include
# SOURCES += \
#     $$PWD/muparser-master/src/muParser.cpp \
#     $$PWD/muparser-master/src/muParserBase.cpp \
#     $$PWD/muparser-master/src/muParserCallback.cpp \
#     $$PWD/muparser-master/src/muParserError.cpp \
#     $$PWD/muparser-master/src/muParserTokenReader.cpp

# project
SOURCES += main.cpp \
           aes.cpp \
           database.cpp \
           func2serv.cpp \
           rsa.cpp \
           server_api.cpp \
           sha384.cpp

HEADERS +=  database.h \
    aes.h \
    func2serv.h \
    rsa.h \
    server_api.h \
    sha384.h

