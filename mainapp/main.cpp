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

#include "mainwindow.h"
#include "svgsplashscreen.h"
// Qt solutions
#include <3rdparty/qtsingleapplication/src/qtsingleapplication.h>
#include <QDebug>
#include <QSvgWidget>
#include <QTimer>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QtSingleApplication app(argc, argv);
	app.setApplicationName("PartyPlayer");
	app.setOrganizationName("BuusSW");
	
	QString message = app.arguments().join("* *");
	qDebug() << app.arguments().count() -1;
	if(app.isRunning())
	{
		app.sendMessage(message);
		return 0;
	}

	SvgSplashScreen splash(":/mainwindow/splash.svg");
	splash.show();
	splash.fadeIn();
	QTimer splashTimer;
	splashTimer.setSingleShot(true);
	
	MainWindow win;
	QObject::connect(&splashTimer,SIGNAL(timeout()),&splash,SLOT(fadeOut()));
	QObject::connect(&splashTimer,SIGNAL(timeout()),&win,SLOT(show()));
	splashTimer.start(5000);
	win.handleApplicationMessage(message);

	QObject::connect(&app,SIGNAL(messageReceived(const QString&)),
					&win,SLOT(handleApplicationMessage(const QString &)));

	app.setActivationWindow(&win, false);
	QObject::connect(&win, SIGNAL(needToShow()), &app, SLOT(activateWindow()));
//	splash.hide();
//	win.show();
	return app.exec();
}

