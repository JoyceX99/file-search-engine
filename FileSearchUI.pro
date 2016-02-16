#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T02:23:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileSearchUI
TEMPLATE = app


SOURCES += main.cpp \
           finder.cpp \
           listdelegate.cpp \
    search/inverted_index.cpp \
    search/ranking.cpp \
    search/transform.cpp

HEADERS  += finder.h \
    listdelegate.h \
    search/inverted_index.h \
    search/ranking.h \
    search/transform.h

FORMS    += finder.ui

INCLUDEPATH += "search/"

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
