
TEMPLATE = subdirs
CONFIG += orderd
unix:subdirs += src/mainapp/3rdparty/qtsingleapplication/buildlib/buildlib.pro
SUBDIRS += 	src/playlist/playlist.pro \
		src/player/player.pro \
    		src/mainapp/mainapp.pro

