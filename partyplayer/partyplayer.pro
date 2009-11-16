TEMPLATE = app
TARGET = partyplayer
DESTDIR = ../Debug
QT += network webkit phonon xml
CONFIG += debug
DEFINES += QT_XML_LIB

INCLUDEPATH += . \
    ./GeneratedFiles/Debug \
    ..
win32:INCLUDEPATH += C:/dev/include/taglib
unix:INCLUDEPATH += /usr/include/taglib

#LIBS
LIBS += -lphonon -ltag -lplaylist -L../Debug 
win32:LIBS += C:/dev/lib/tag.lib
unix:LIBS += -ltag -L/usr/lib

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(partyplayer.pri)
