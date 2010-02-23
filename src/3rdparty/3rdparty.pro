
unix {
	TEMPLATE=lib
	CONFIG += qt dll qtsingleapplication-buildlib

	TARGET = QtSolutions_SingleApplication-2.6

	QT += network

	include(../common.pri)

	SOURCES += 	./qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.cpp \
			./qtsingleapplication-2.6_1-opensource/src/qtlocalpeer.cpp
	HEADERS += 	./qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.h \
			./qtsingleapplication-2.6_1-opensource/src/qtlocalpeer.h
}
			
