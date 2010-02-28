/*
** This file is part of the Party Player application.
** Copyright (C) 2009  Anders Buus
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifdef NO_SPLASH
#include <QApplication>
#include "mainwindow.h"
#else
#include "application.h"
#endif


int main(int argc, char *argv[])
{
#ifdef NO_SPLASH
	QApplication app(argc,argv);
	MainWindow w;
	w.show();
	return app.exec();
#else
	Application app(argc,argv);
#ifdef Q_WS_WIN 
	app.addLibraryPath(app.applicationDirPath());
	app.addLibraryPath(app.applicationDirPath() + "/bin/taglib");
	app.addLibraryPath(app.applicationDirPath() + "/bin/gstreamer/bin");
	app.addLibraryPath(app.applicationDirPath() + "/bin/plugins");
#elif defined Q_WS_X11
	app.addLibraryPath("./");
#endif
	return app.exec();
#endif
}

