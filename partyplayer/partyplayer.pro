# -----------------------------------------------------------
# This file is generated by the Qt Visual Studio Integration.
# -----------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


TEMPLATE = app
TARGET = partyplayer
DESTDIR = ../Debug
QT += network webkit phonon
CONFIG += debug
DEFINES += QT_XML_LIB
INCLUDEPATH += . \
    ./GeneratedFiles/Debug \
    /usr/include/taglib
LIBS += -lphonon -ltag -L/usr/lib
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(partyplayer.pri)
