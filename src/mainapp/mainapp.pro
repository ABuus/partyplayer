TEMPLATE = app
TARGET = partyplayer

CONFIG(debug, debug|release) {
DESTDIR = ../../debug
unix:QMAKE_POST_LINK += cp 3rdparty/qtsingleapplication/lib/* ../../debug
win32:QMAKE_POST_LINK += copy 3rdparty\qtsingleapplication\lib\* ..\..\debug
} else {
DESTDIR = ../../release
unix:QMAKE_POST_LINK += cp 3rdparty/qtsingleapplication/lib/* ../../release
win32:QMAKE_POST_LINK += copy 3rdparty\qtsingleapplication\lib\* ..\..\release
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
unix:INCLUDEPATH += /usr/include \
	/usr/include/gstreamer-0.10 \
	/usr/include/glib-2.0 \
	/usr/include/libxml2 \
	/usr/lib64/glib-2.0/include \
	/usr/include/taglib \
	3rdparty/qtsingleapplication/src
unix:LIBS += -L$(DESTDIR) \
	-L3rdparty/qtsingleapplication/lib \
	-lplaylist \
	-lplayer \
	-lQtSolutions_SingleApplication-2.6 
win32:LIBS += -L$$DESTDIR \
	-L3rdparty/qtsingleapplication/lib \
	C:/dev/lib/tag.lib \
	C:/gstreamer-dev/lib/gstreamer-0.10.lib \
	C:/gstreamer-dev/lib/glib-2.0.lib \
	C:/gstreamer-dev/lib/gobject-2.0.lib \
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
win32:include(3rdparty/qtsingleapplication/src/qtsingleapplication.pri)
