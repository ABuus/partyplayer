TEMPLATE = lib
TARGET = playlist

QT += network xml

include(../common.pri)

DEFINES += PLAYLIST_LIB

LIBS += -L$$DESTDIR \
		-lplaylistitem

win32 {
INCLUDEPATH += $(QTDIR)/mkspecs/win32-msvc2008
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += taglib
}

#Include file(s)
include(playlist.pri)
