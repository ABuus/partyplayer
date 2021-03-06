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

#include "application.h"
#include "mainwindow.h"
#include "svgsplashscreen.h"


Application::Application(int &argc, char * argv[])
	: QtSingleApplication(argc,argv)
{
	
	loadOnlineInfo = true;

	setApplicationName("PartyPlayer");
	setOrganizationName("PartyPlayer");

	settings = new QSettings(organizationName(),applicationName(),this);
	settings->setDefaultFormat(QSettings::IniFormat);
	splash = new SvgSplashScreen(":/mainwindow/splash.svg");
	splashTimer = new QTimer(this);
	m_mainWindow = new MainWindow();

	QStringList arg(arguments());
	arg.removeFirst();
	QString message = arg.join("* *");

	if(isRunning())
	{
		Debug << "sending message to existing procces";
		sendMessage(message);
		return;
	}
	splash->show();
	splash->fadeIn();
	splashTimer->setSingleShot(true);
	
	connect(splashTimer,SIGNAL(timeout()),splash,SLOT(fadeOut()));
	connect(splashTimer,SIGNAL(timeout()),m_mainWindow,SLOT(show()));
	splashTimer->start(5000);
	m_mainWindow->handleApplicationMessage(message);

	connect(this,SIGNAL(messageReceived(const QString&)),
					m_mainWindow,SLOT(handleApplicationMessage(const QString &)));

	setActivationWindow(m_mainWindow, false);
	connect(m_mainWindow, SIGNAL(needToShow()), this, SLOT(activateWindow()));
}

Application::~Application()
{
	delete m_mainWindow;
}

