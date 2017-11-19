#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T15:36:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++1z
DEFINES += DEBUG
TARGET = TeQtGameOfHexes
TEMPLATE = app


SOURCES += main.cpp \
    game.cpp \
    hex.cpp \
    random.cpp \
    hexboard.cpp \
    button.cpp

HEADERS  += \
    game.hpp \
    hex.hpp \
    random.h \
    hexboard.hpp \
    button.hpp

FORMS    +=
