

unix {
SUBDIRS += src/3rdparty/3rdparty.pro
src/mainapp.depends += src/3rdparty/3rdparty.pro
}

win32 {
src/playlistitem.depends += src/3rdparty/taglib-1.6.1/taglib
SUBDIRS += 	src/3rdparty/qtsingleapplication-2.6_1-opensource/buildlib \
			src/3rdparty/taglib-1.6.1/taglib
src/mainapp.depends += src/3rdparty/qtsingleapplication-2.6_1-opensource/buildlib
}

src/playlist.depends += src/playlistitem
src/mainapp.depends += src/playlist src/player


TEMPLATE = subdirs
CONFIG += orderd
SUBDIRS += src/playlistitem \
	src/playlist \
	src/player \
	src/mainapp
	
