#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T17:24:31
#
#-------------------------------------------------

QT       += core gui

TARGET = QtVtkStarter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS    +=  -L/usr/lib/ -lvtkCommon\
            -lvtksys -lQVTK -lvtkViews -lvtkWidgets\
            -lvtkInfovis -lvtkRendering -lvtkGraphics\
            -lvtkImaging -lvtkIO -lvtkFiltering\
            -lvtkDICOMParser -lvtkHybrid

INCLUDEPATH += /usr/include/vtk-5.8/

RESOURCES += \
    Zasoby.qrc

