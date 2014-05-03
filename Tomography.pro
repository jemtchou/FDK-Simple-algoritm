#-------------------------------------------------
#
# Project created by QtCreator 2014-03-02T11:55:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -ltiff -lm -O2
QMAKE_LFLAGS += -ltiff -lm -O2

TARGET = Tomography
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintwidget.cpp \
    fileopen.cpp \
    corefdksimple.cpp

HEADERS  += mainwindow.h \
    paintwidget.h \
    fileopen.h \
    corefdksimple.h \
    imgsave.h

FORMS    += mainwindow.ui
