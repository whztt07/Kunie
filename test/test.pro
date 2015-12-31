include(../opencascade.pri)
include(../core/core.pri)

QT += core gui widgets testlib

CONFIG += testcase

TARGET = testapp
TEMPLATE = app

DEFINES += DATA=\\\"$$PWD/../data\\\"

SOURCES += \
    main.cpp \
    Test.cpp

HEADERS += \
    Test.h
