unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    BINDIR = $$PREFIX/bin
 
    INSTALLS += target
    target.path = $$BINDIR
 
    DATADIR = $$PREFIX/share
}

unix {
DEFINES += FLASH_NO_RESIZE
}

DATADIR = $$PWD/../build/objects
MOC_DIR = $$DATADIR
UI_DIR = $$DATADIR
RCC_DIR = $$DATADIR
OBJECTS_DIR = $$DATADIR

DESTDIR = $$PWD/../build
DEPENDPATH += $$DESTDIR $$DATADIR
INCLUDEPATH += $$PWD