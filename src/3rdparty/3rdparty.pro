
unix {
	TEMPLATE=lib
	CONFIG += qt dll qtsingleapplication-buildlib

	TARGET = QtSolutions_SingleApplication-2.6

	QT += network

	SOURCES += 	src/3rdparty/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.cpp \
			src/3rdparty/qtsingleapplication-2.6_1-opensource/src/qtlocalpeer.cpp
	HEADERS += 	src/3rdparty/qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.h \
			src/3rdparty/qtsingleapplication-2.6_1-opensource/src/qtlocalpeer.h
}
			
