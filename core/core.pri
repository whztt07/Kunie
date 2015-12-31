win32 {
    CONFIG(release, debug|release): libdirsuffix = release
    CONFIG(debug, debug|release): libdirsuffix = debug
    LIB_DIR = $$clean_path($$shadowed($$PWD/$$libdirsuffix))
}

unix:LIB_DIR = $$clean_path($$shadowed($$PWD))

INCLUDEPATH += $$PWD
DEPENDPATH = $$INCLUDEPATH


LIBS += -L$$LIB_DIR -lcore

PRE_TARGETDEPS += $$LIB_DIR/libcore.a
