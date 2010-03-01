include(../common.pri)

TEMPLATE = lib
TARGET = playlistitem
QT += network xml

DEFINES += QT_LARGEFILE_SUPPORT QT_XML_LIB QT_NETWORK_LIB PLAYLISTITEM_LIB
INCLUDEPATH += . 
DEPENDPATH += .

win32 {
INCLUDEPATH += c:/dev/include/taglib
LIBS += c:/dev/lib/tag.lib
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += taglib
}

include(playlistitem.pri)
