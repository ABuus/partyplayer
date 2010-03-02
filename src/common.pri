
CONFIG(debug) {
DEFINES += DEBUG_BUILD
}

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    BINDIR = $$PREFIX/bin
 
    INSTALLS += target
    target.path = $$BINDIR
 
	DEFINES += FLASH_NO_RESIZE

	DATADIR = $$PWD/../build/objects
	MOC_DIR = $$DATADIR
	UI_DIR = $$DATADIR
	RCC_DIR = $$DATADIR
	OBJECTS_DIR = $$DATADIR
	DESTDIR = $$PWD/../build
	DEPENDPATH += $$DESTDIR $$DATADIR
	INCLUDEPATH += .
	QMAKE_RPATH += $$DESTDIR
}

win32 {
TAGLIBDIR = "C:\dev\taglib"
DESTDIR = ../../build
DEPENDPATH += $$DESTDIR
INCLUDEPATH += .
}
