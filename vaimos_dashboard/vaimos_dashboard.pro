QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vaimos_dashboard
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    widgetVaimos.cpp

HEADERS  += mainwindow.h \
    widgetVaimos.h

FORMS    += mainwindow.ui
