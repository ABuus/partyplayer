TEMPLATE = subdirs
SUBDIRS = 	src/3rdparty/qtsingleapplication-2.6_1-opensource/buildlib \
		src

src.depends = src/3rdparty/qtsingleapplication-2.6_1-opensource/buildlib

OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui

