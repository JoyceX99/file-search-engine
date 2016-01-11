#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T02:23:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileSearchUI
TEMPLATE = app


SOURCES += main.cpp\
        finder.cpp \
    listdelegate.cpp \
    ../../XCodeProjects/FileSearch/FileSearch/inverted_index.cpp \
    ../../XCodeProjects/FileSearch/FileSearch/ranking.cpp \
    ../../XCodeProjects/FileSearch/FileSearch/transform.cpp
    ../../XCodeProjects/FileSearch/FileSearch/ranking.cpp
    ../../XCodeProjects/FileSearch/FileSearch/transform.cpp

HEADERS  += finder.h \
    listdelegate.h \
    ../../XCodeProjects/FileSearch/FileSearch/inverted_index.h \
    ../../XCodeProjects/FileSearch/FileSearch/ranking.h \
    ../../XCodeProjects/FileSearch/FileSearch/transform.h
    ../../XCodeProjects/FileSearch/FileSearch/ranking.h
    ../../XCodeProjects/FileSearch/FileSearch/transform.h

FORMS    += finder.ui

INCLUDEPATH += "/Users/Joyce/Documents/XCodeProjects/FileSearch/FileSearch"

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
