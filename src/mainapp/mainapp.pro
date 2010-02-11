TEMPLATE = app
TARGET = partyplayer
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10 taglib
CONFIG(debug, debug|release) {
	DESTDIR = ../../debug
} else {
	DESTDIR = ../../release
}

QT += network xml webkit core gui svg

INCLUDEPATH += . \
    debug \
    .. 

win32:INCLUDEPATH += C:/dev/include/taglib \
	C:/gstreamer-dev/include/gstreamer-0.10 \
	C:/gstreamer-dev/include/glib-2.0 \
	C:/gstreamer-dev/include/libxml2 \
	C:/gstreamer-dev/lib/glib-2.0/include \
	C:/gstreamer-dev/include \
	3rdparty/qtsingleapplication/src
unix:INCLUDEPATH += /usr/include 
unix:LIBS += -L$(DESTDIR) \
	-lplaylist \
	-lplayer \
	-lQtSolutions_SingleApplication-2.6 
win32:LIBS += -L$(DESTDIR) \
	-lQtSolutions_SingleApplication-2.6 \
	-lplaylist \
	-lplayer
RESOURCES     = mainwindow.qrc	
RC_FILE = partyplayer.rc
DEPENDPATH += .
MOC_DIR += debug
OBJECTS_DIR += debug
UI_DIR += debug
RCC_DIR += debug

#Include file(s)
include(mainapp.pri)
include(3rdparty/qtsingleapplication/src/qtsingleapplication.pri)
