TEMPLATE = lib
TARGET = playlist
DESTDIR = ../Debug
CONFIG += debug
DEFINES += PLAYLIST_LIB

#INCLUDEPATH
INCLUDEPATH += . \
    ./GeneratedFiles/Debug
win32:INCLUDEPATH += C:/dev/include/taglib
unix:INCLUDEPATH += /usr/include/taglib

#LIBS
unix:LIBS += -ltag
win32:LIBS += C:/dev/lib/tag.lib

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(playlist.pri)
