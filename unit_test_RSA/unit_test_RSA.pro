QT += testlib network gui
CONFIG += qt console testcase moc
CONFIG += console
CONFIG -= windows
INCLUDEPATH += C:/msys64/mingw64/include
LIBS += -LC:/msys64/mingw64/lib -lgmp -lgmpxx
TEMPLATE = app

SOURCES += main.cpp \
           C:/QTCreator/projects/RSA/rsa.cpp


HEADERS += \ C:/QTCreator/projects/RSA/rsa.h
