CONFIG(debug, debug|release) { 
QTSINGLEAPPLICATION_LIBDIR = debug
} else {
QTSINGLEAPPLICATION_LIBDIR = release
}
TEMPLATE = subdirs
CONFIG += orderd
SUBDIRS += 	src/playlist/playlist.pro \
		src/player/player.pro \
		src/mainapp/3rdparty/qtsingleapplication/buildlib/buildlib.pro \
    		src/mainapp/mainapp.pro

