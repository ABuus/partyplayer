# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

HEADERS += 	./player_global.h \
		./YoutubePlayer/youtubeplayer.h
SOURCES += 	./YoutubePlayer/youtubeplayer.cpp

win32 {
	HEADERS += 	./DShowPlayer/qtdirectaudio.h \
			./DShowPlayer/qtdirectaudio_p
	SOURCES += 	./DShowPlayer/qtdirectaudio.cpp
}

unix {	
	HEADERS += 	./GstPlayer/qgstplayer.h \
			./GstPlayer/qgstplayer_p
	SOURCES += 	./GstPlayer/qgstplayer.cpp
}
