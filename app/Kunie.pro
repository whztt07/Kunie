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
    SphereCommand.cpp

HEADERS += \
    MainWindow.h \
    OccView.h \
    Document.h \
    Translator.h \
    CylinderDriver.h \
    Application.h \
    CylinderCommand.h \
    SphereDriver.h \
    SphereCommand.h

QMAKE_CXXFLAGS += -isystem $$CASROOT/inc

INCLUDEPATH += $$CASROOT/inc
DEPENDPATH = $$INCLUDEPATH

LIBS += -L$$CASROOT/lin64/gcc/libd

LIBS += -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d -lTKOpenGl \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset -lTKCAF -lTKLCAF

RESOURCES += \
    resources.qrc
