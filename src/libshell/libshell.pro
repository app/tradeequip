include ( ../config.pri ) 
TEMPLATE = lib
TARGET = teshell
CONFIG += qt warn_on console
LANGUAGE = C++
INCLUDEPATH += ../lib
SOURCES = teshell.cpp
HEADERS = teshell.h
LIBS += -L../../lib -ltradeequip
DESTDIR     = ../../lib
DEFINES += _CRT_SECURE_NO_DEPRECATE 
