
INCLUDEPATH += playlist \
win32:INCLUDEPATH += C:/dev/include/taglib
unix:INCLUDEPATH += /usr/include/taglib

#LIBS
unix:LIBS += -ltag
win32:LIBS += C:/dev/lib/tag.lib

CONFIG += ordered
TEMPLATE = subdirs

SUBDIRS += 	playlist/playlist.pro \
    		partyplayer/partyplayer.pro 
