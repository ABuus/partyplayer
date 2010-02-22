TEMPLATE = lib
TARGET = playlist

QT += network

include(../common.pri)

DEFINES += PLAYLIST_LIB

win32 {
INCLUDEPATH += $(QTDIR)/mkspecs/win32-msvc2008 \
			c:/dev/include/taglib
LIBS += c:/dev/lib/tag.lib
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += taglib
}



#Include file(s)
include(playlist.pri)
