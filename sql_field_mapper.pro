#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T13:53:30
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sql_field_mapper
TEMPLATE = app


SOURCES += main.cpp \
    logindialog.cpp \
    vwindow.cpp \
    mapwindow.cpp \
    querywindow.cpp \
    queryeditor.cpp

HEADERS  += \
    logindialog.h \
    vwindow.h \
    mapwindow.h \
    querywindow.h \
    queryeditor.h

FORMS += \
    vwindow.ui \
    mapwindow.ui \
    querywindow.ui
