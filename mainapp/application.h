#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <3rdparty/qtsingleapplication/src/qtsingleapplication.h>
#include <QDebug>
#include <QTimer>
#include <QDesktopWidget>
#include <QProcess>
#include "mainwindow.h"
#include "svgsplashscreen.h"

class Application : public QtSingleApplication
{
	Q_OBJECT

public:
	Application( int argc, char *argv[]);
	~Application();
#ifdef Q_WS_WIN
	bool setWinEnv(const QString appdir);
#endif
private:
	MainWindow *win;
	SvgSplashScreen *splash;
	QTimer *splashTimer;
};

#endif // APPLICATION_H
