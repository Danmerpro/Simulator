#-------------------------------------------------
#
# Project created by QtCreator 2013-03-23T01:29:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RLS_IKO
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    simulation.cpp \
    routepoint.cpp \
    mapobj.cpp \
    connectionmenu.cpp \
    pointslist.cpp

HEADERS  += mainwindow.h \
    simulation.h \
    routepoint.h \
    mapobj.h \
    connectionmenu.h \
    pointslist.h
