QT += core gui widgets testlib

CONFIG += testcase

TARGET = testKunie
TEMPLATE = app

CASROOT = $$(CASROOT)

isEmpty(CASROOT) {
    error("CASROOT environment variable is not set")
}

SOURCES += \
    main.cpp \
    ../app/MainWindow.cpp \
    ../app/OccView.cpp \
    ../app/MakeBottle.cpp \
    ../app/Document.cpp \
    ../app/Translator.cpp \
    KunieTest.cpp

HEADERS += \
    ../app/MainWindow.h \
    ../app/OccView.h \
    ../app/Document.h \
    ../app/Translator.h \
    KunieTest.h

QMAKE_CXXFLAGS += -isystem $$CASROOT/inc

INCLUDEPATH += ../app $$CASROOT/inc
DEPENDPATH = $$INCLUDEPATH

LIBS += -L$$CASROOT/lin64/gcc/libd

LIBS += -lTKernel -lPTKernel -lTKMath -lTKService -lTKV3d -lTKOpenGl \
        -lTKBRep -lTKIGES -lTKSTL -lTKVRML -lTKSTEP -lTKSTEPAttr -lTKSTEP209 \
        -lTKSTEPBase -lTKShapeSchema -lTKGeomBase -lTKGeomAlgo -lTKG3d -lTKG2d \
        -lTKXSBase -lTKPShape -lTKShHealing -lTKHLR -lTKTopAlgo -lTKMesh -lTKPrim \
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset

RESOURCES += \
    ../app/resources.qrc
