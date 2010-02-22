TEMPLATE=lib
CONFIG += qt dll qtsingleapplication-buildlib

TARGET = QtSolutions_SingleApplication-2.6

QT += network

include(../common.pri)

SOURCES += $$PWD/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.cpp \
	   $$PWD/qtsingleapplication-2.6_1-opensource/src/qtlocalpeer.cpp
HEADERS += $$PWD/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.h \
	   $$PWD/qtsingleapplication-2.6_1-opensource/src/qtlocalpeer.h