QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HTYFTP
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ftpmanager.cpp

HEADERS += \
        mainwindow.h \
    ftpmanager.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    res.qrc
