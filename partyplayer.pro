

TEMPLATE = subdirs
CONFIG += orderd
unix {
SUBDIRS += 	src/playlist/playlist.pro \
			src/player/player.pro \
			src/3rdparty/3rdparty.pro \
    		src/mainapp/mainapp.pro
}

win32 {
SUBDIRS += 	src/3rdparty/qtsingleapplication-2.6_1-opensource/buildlib/buildlib.pro \
			src/playlist/playlist.pro \
			src/player/player.pro \
			src/mainapp/mainapp.pro
}

