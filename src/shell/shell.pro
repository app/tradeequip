include ( ../config.pri ) 

TEMPLATE = app
TARGET = teshell

CONFIG += qt warn_on console

LANGUAGE = C++

INCLUDEPATH += ../lib

SOURCES += main.cpp

HEADERS += receiver.h

LIBS += -L../../lib -ltradeequip

win32 {
#LIBS += -L../../../cl32/binaries -lcl32
}

unix {
LIBS += -lpthread
}

DESTDIR     = ../../bin
DEFINES += _CRT_SECURE_NO_DEPRECATE 
