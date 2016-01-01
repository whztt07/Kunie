include(../opencascade.pri)

QT += core gui widgets

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    MainWindow.cpp \
    OccView.cpp \
    MakeBottle.cpp \
    Document.cpp \
    Translator.cpp \
    CylinderDriver.cpp \
    Application.cpp \
    CylinderCommand.cpp \
    SphereDriver.cpp \
    SphereCommand.cpp \
    CutDriver.cpp \
    CutCommand.cpp \
    LabelItem.cpp \
    AttributeItem.cpp \
    Color.cpp

HEADERS += \
    MainWindow.h \
    OccView.h \
    Document.h \
    Translator.h \
    CylinderDriver.h \
    Application.h \
    CylinderCommand.h \
    SphereDriver.h \
    SphereCommand.h \
    CutDriver.h \
    CutCommand.h \
    LabelItem.h \
    AttributeItem.h \
    Color.h

RESOURCES += \
    resources.qrc
