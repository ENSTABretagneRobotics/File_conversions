QT -= core
QT -= gui

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

TARGET = NMEA2CSV_WRSC2018

DEFINES -= UNICODE
win32:DEFINES += WIN32 _CONSOLE
win32:DEFINES += ENABLE_GETTIMEOFDAY_WIN32

SOURCES += \
    Main.c \
    OSMisc.c \
    OSTime.c \
    OSCore.c

HEADERS += \
    OSMisc.h \
    OSTime.h \
    OSCore.h
