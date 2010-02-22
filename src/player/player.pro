TEMPLATE = lib
TARGET = player

QT += webkit network gui core

include(../common.pri)

DEFINES += PLAYER_LIB

win32 {
INCLUDEPATH += 	"C:\gstreamer-dev\include" \
		"C:\gstreamer-dev\include/glib-2.0" \
		"C:\gstreamer-dev\include/glib-2.0" \
		"C:\gstreamer-dev\include/libxml2" \
		"C:\gstreamer-dev\include/gstreamer-0.10" \
		"C:\gstreamer-dev\lib\glib-2.0\include" \
		./GeneratedFiles/Debug
LIBS += 	"C:\gstreamer-dev\lib\gstreamer-0.10.lib" \
		"C:\gstreamer-dev\lib\glib-2.0.lib" \
		"C:\gstreamer-dev\lib\gobject-2.0.lib"
QT += phonon
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10 phonon
}


#Include file(s)
include(player.pri)
