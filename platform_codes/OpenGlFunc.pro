#-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T22:10:16
#
#-------------------------------------------------

QT       += core gui opengl script network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += uic3

TARGET = OpenGlFunc
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    glwidget.cpp \
    setting.cpp \
    infowrapper.cpp \
    tinyVM.cpp

HEADERS += \
    glwidget.h \
    setting.h \
    requestinfo.h \
    infowrapper.h \
    tinyVM.h

FORMS += \
        mainwindow.ui \
    setting.ui

DISTFILES += \
    vsshader.vert \
    fgshader.frag \
    funcvert.vert \
    marchingCubes.compute \
    marchingCubesRearrange.compute \
    marchingcube.gtpl
