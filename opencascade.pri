CASROOT = $$(CASROOT)

isEmpty(CASROOT) {
    error("CASROOT environment variable is not set")
}

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
        -lTKCDF -lTKBool -lTKBO -lTKFillet -lTKOffset -lTKCAF -lTKLCAF \
        -lTKXCAF -lTKXDESTEP -lTKXDEIGES
