TEMPLATE = lib
TARGET = playlist
DESTDIR = ../Debug
CONFIG += debug
DEFINES += PLAYLIST_LIB
INCLUDEPATH += . \
    ./GeneratedFiles/Debug \
win32:INCLUDEPATH += ../taglib/taglib
unix:LIBS += -ltag
win32:LIBS += ../taglib-1.5-msvc-bin/lib/tag.lib
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(playlist.pri)
