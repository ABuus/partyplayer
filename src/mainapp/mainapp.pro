TEMPLATE = app
TARGET = partyplayer


QT += network xml webkit core gui svg

include(../common.pri)

unix {

INCLUDEPATH += /usr/include \
	3rdparty/qtsingleapplication/src
LIBS += -L$(DESTDIR) \
	-lplaylist \
	-lplayer \
	-lplaylistitem \
	-lQtSolutions_SingleApplication-2.6 
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-0.10 taglib

}

win32 {
LIBS += -L$$DESTDIR \
		-Lsrc/3rdparty/qtsingleapplication-2.6_1-opensource/lib/ \
		-lplaylist \
		-lplayer
INCLUDEPATH += $$TAGLIBDIR/include/taglib \
	C:/gstreamer-dev/include/gstreamer-0.10 \
	C:/gstreamer-dev/include/glib-2.0 \
	C:/gstreamer-dev/include/libxml2 \
	C:/gstreamer-dev/lib/glib-2.0/include \
	C:/gstreamer-dev/include \
	../3rdparty/qtsingleapplication/src

	
RESOURCES     = mainwindow.qrc	
RC_FILE = partyplayer.rc
}



#Include file(s)
include(mainapp.pri)
win32:include(../3rdparty/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.pri)

