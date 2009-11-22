TEMPLATE = app
TARGET = partyplayer

CONFIG(debug) {
DESTDIR = ../debug
} else {
DESTDIR = ../release
}

QT += network xml webkit core gui

#DEFINES += QT_LARGEFILE_SUPPORT QT_XML_LIB QT_DLL QT_WEBKIT_LIB QT_PHONON_LIB QT_NETWORK_LIB
INCLUDEPATH += . \
    debug \
    .. \
    $(QTDIR)/mkspecs/win32-msvc2008

win32:INCLUDEPATH += C:/dev/include/taglib \
	C:/gstreamer/include/gstreamer-0.10 \
	C:/gstreamer/include/glib-2.0 \
	C:/gstreamer/include/libxml2 \
	C:/gstreamer/lib/glib-2.0/include
unix:INCLUDEPATH += /usr/include \
	/usr/include/gstreamer-0.10 \
	/usr/include/glib-2.0 \
	/usr/include/libxml2 \
	/usr/lib64/glib-2.0/include \
	/usr/include/taglib
unix:LIBS += -L$(DESTDIR) \
	-lplaylist \
	-lplayer 

win32:LIBS += -L../debug \
	-L../release \
	C:/dev/lib/tag.lib \
	C:/gstreamer/lib/gstreamer.lib \
	C:/gstreamer/lib/glib-2.0.lib \
	C:/gstreamer/lib/gobject-2.0.lib \
	-lplaylist \
	-lplayer

RESOURCES     = mainwindow.qrc	

DEPENDPATH += .
MOC_DIR += debug
OBJECTS_DIR += debug
UI_DIR += debug
RCC_DIR += debug

#Include file(s)
include(mainapp.pri)
