include(../common.pri)

TEMPLATE = lib
TARGET = playlistitem
QT += network xml

DEFINES += QT_LARGEFILE_SUPPORT QT_XML_LIB QT_NETWORK_LIB PLAYLISTITEM_LIB
INCLUDEPATH += . 
DEPENDPATH += .

win32 {
INCLUDEPATH += $$TAGLIBDIR \
              $$TAGLIBDIR/ape \
              $$TAGLIBDIR/flac \
              $$TAGLIBDIR/mpc \
              $$TAGLIBDIR/mp4 \
              $$TAGLIBDIR/mpeg \
              $$TAGLIBDIR/ogg \
              $$TAGLIBDIR/ogg/speex \
              $$TAGLIBDIR/toolkit \
              $$TAGLIBDIR/trueaudio \
              $$TAGLIBDIR/wavpack \
              $$TAGLIBDIR/mpeg/id3v1 \
              $$TAGLIBDIR/mpeg/id3v2 \
              $$TAGLIBDIR/ogg/flac \
              $$TAGLIBDIR/ogg/vorbis \
              $$TAGLIBDIR/mpeg/id3v2/frames \
			  $$TAGLIBDIR/riff \
			  $$TAGLIBDIR/riff/aiff \
			  $$TAGLIBDIR/riff/wav \
			  $$TAGLIBDIR/asf
			  
LIBS += -L$$DESTDIR \
		-lTagLib1
}

unix {
CONFIG += link_pkgconfig
PKGCONFIG += taglib
}

include(playlistitem.pri)
