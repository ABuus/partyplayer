TEMPLATE = app
TARGET = partyplayer
DESTDIR = ../Debug
QT += network webkit phonon xml
CONFIG += debug
DEFINES += QT_XML_LIB
INCLUDEPATH += . \
    ./GeneratedFiles/Debug \
    ..
unix:INCLUDEPATH += /usr/include/taglib \
	/usr/include/taglib
win32:INCLUDEPATH += ../taglib-1.5-msvc-bin/include/taglib 
LIBS += -lphonon -ltag -lplaylist -L../Debug 
unix:LIBS += -L/usr/lib
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(partyplayer.pri)
