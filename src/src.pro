
TARGET = partyplayer
TEMPLATE = app
QT += webkit network gui core xml svg

INCLUDEPATH += $$PWD/src/mainapp \
		$$PWD/src/player \
		$$PWD/src/playlist \
		$$PWD/src/playlistitem \
		3rdparty/qtsingleapplication-2.6_1-opensource/src
DEPENDSPATH += $INCLUDEPATH


include(mainapp/mainapp.pri)
include(player/player.pri)
include(playlist/playlist.pri)
include(playlistitem/playlistitem.pri)



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
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10 taglib
}

LIBS += -L3rdparty/qtsingleapplication-2.6_1-opensource/lib -lQtSolutions_SingleApplication-2.6


