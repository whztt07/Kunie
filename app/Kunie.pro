QT += core gui widgets

TARGET = Kunie
TEMPLATE = app

CASROOT = $$(CASROOT)

isEmpty(CASROOT) {
    error("CASROOT environment variable is not set")
}

SOURCES += \
    main.cpp \
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
    OcafModel.cpp \
    OcafItem.cpp \
    LabelItem.cpp \
    AttributeItem.cpp

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
    OcafModel.h \
    OcafItem.h \
    LabelItem.h \
    AttributeItem.h

linux:QMAKE_CXXFLAGS += -isystem $$CASROOT/inc
win32:QMAKE_CXXFLAGS += -isystem C:\\OCE\\OCE-0.17\\include\\oce

linux:INCLUDEPATH += $$CASROOT/inc
win32:INCLUDEPATH += C:\\OCE\\OCE-0.17\\include\\oce
DEPENDPATH = $$INCLUDEPATH

linux:LIBS += -L$$CASROOT/lin64/gcc/libd
win32:LIBS += -LC:\\OCE\\OCE-0.17\\Win32\\bin

LIBS += -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d -lTKOpenGl \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset -lTKCAF -lTKLCAF

RESOURCES += \
    resources.qrc
