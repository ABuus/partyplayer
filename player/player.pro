
TEMPLATE = lib
TARGET = player
CONFIG(debug, debug|release) {
DESTDIR = ../debug
} else {
DESTDIR = ../release
}

DEFINES += PLAYER_LIB

win32:INCLUDEPATH += c:/gstreamer/lib/glib-2.0/include \
    c:/gstreamer/include/glib-2.0 \
    c:/gstreamer/include/libxml2 \
    c:/gstreamer/include/gstreamer-0.10 \
    . \
    ./GeneratedFiles/Debug
win32:LIBS += C:/gstreamer/lib/gstreamer.lib \
    C:/gstreamer/lib/glib-2.0.lib \
    C:/gstreamer/lib/gobject-2.0.lib

unix:INCLUDEPATH += /usr/include/gstreamer-0.10 \
	/usr/include/glib-2.0 \
	/usr/include/libxml2 \
	/usr/lib64/glib-2.0/include

unix:LIBS +=  $$system(pkg-config gstreamer-0.10 --libs) 

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(player.pri)
