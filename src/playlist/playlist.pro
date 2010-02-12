
TEMPLATE = lib
TARGET = playlist
QT += network

CONFIG(debug, debug|release) {
DESTDIR = ../../debug
} else {
DESTDIR = ../../release
}

#win32:CONFIG += staticlib

DEFINES += PLAYLIST_LIB

INCLUDEPATH += . \
    ./GeneratedFiles/Debug

win32:INCLUDEPATH += $(QTDIR)/mkspecs/win32-msvc2008 \
			c:/dev/include/taglib
unix:INCLUDEPATH += /usr/include/taglib

unix:LIBS += $$system(pkg-config taglib --libs)
win32:LIBS += c:/dev/lib/tag.lib

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(playlist.pri)
