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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QtSingleApplication>
//#include <QtLocalpeer>
#include <QTimer>
#include <QSettings>

class MainWindow;
class SvgSplashScreen;

class Application : public QtSingleApplication
{
	Q_OBJECT

public:
	Application( int &argc, char *argv[]);
	~Application();
	static MainWindow * mainWindow();
	bool loadOnlineInfo;
	QSettings *settings;
	static Application * instance() { return static_cast<Application*>(QApplication::instance()); };
private:
	MainWindow *m_mainWindow;
	SvgSplashScreen *splash;
	QTimer *splashTimer;
};

#endif // APPLICATION_H
