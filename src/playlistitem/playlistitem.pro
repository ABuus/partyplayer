include(../common.pri)

TEMPLATE = lib
TARGET = playlistitem
QT += network xml

DEFINES += QT_LARGEFILE_SUPPORT QT_XML_LIB QT_NETWORK_LIB PLAYLISTITEM_LIB
INCLUDEPATH += . 
DEPENDPATH += .

win32 {
INCLUDEPATH += $$TAGLIBDIR/include/taglib
LIBS += -L$$TAGLIBDIR/lib \
	-ltag
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += taglib
}

include(playlistitem.pri)
