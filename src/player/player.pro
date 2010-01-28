
QT += webkit network gui core
TEMPLATE = lib
TARGET = player
CONFIG(debug, debug|release) {
DESTDIR = ../../debug
} else {
DESTDIR = ../../release
}

DEFINES += PLAYER_LIB

win32:INCLUDEPATH += "C:\gstreamer-dev\include" \
	"C:\gstreamer-dev\include/glib-2.0" \
    "C:\gstreamer-dev\include/glib-2.0" \
    "C:\gstreamer-dev\include/libxml2" \
    "C:\gstreamer-dev\include/gstreamer-0.10" \
	"C:\gstreamer-dev\lib\glib-2.0\include" \
    . \
    ./GeneratedFiles/Debug
win32:LIBS += "C:\gstreamer-dev\lib\gstreamer-0.10.lib" \
	"C:\gstreamer-dev\lib\glib-2.0.lib" \
	"C:\gstreamer-dev\lib\gobject-2.0.lib"

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
