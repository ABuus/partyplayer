TEMPLATE = subdirs
CONFIG += orderd
SUBDIRS += 	playlist/playlist.pro \
		player/player.pro \
		mainapp/3rdparty/qtsingleapplication/buildlib/buildlib.pro \
    		mainapp/mainapp.pro
CONFIG(debug, debug|release) { 
unix:system(cp mainapp/3rdparty/qtsingleapplication/lib/* debug)
} else {
unix:system(cp mainapp/3rdparty/qtsingleapplication/lib/* release)
}

