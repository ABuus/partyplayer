#include "application.h"

Application::Application(int argc, char * argv[])
	: QtSingleApplication(argc,argv)
{
	setApplicationName("PartyPlayer");
	setOrganizationName("PartyPlayer");
	splash = new SvgSplashScreen(":/mainwindow/splash.svg");
	splashTimer = new QTimer(this);
	win = new MainWindow;

	QStringList arg(arguments());
	arg.removeFirst();
	QString message = arg.join("* *");

	qDebug() << arg.count() -1;
	if(isRunning())
	{
		qDebug() << "sending message to existing procces";
		sendMessage(message);
		return;
	}
	splash->show();
	splash->fadeIn();
	splashTimer->setSingleShot(true);
	
	connect(splashTimer,SIGNAL(timeout()),splash,SLOT(fadeOut()));
	connect(splashTimer,SIGNAL(timeout()),win,SLOT(show()));
	splashTimer->start(5000);
	win->handleApplicationMessage(message);

	connect(this,SIGNAL(messageReceived(const QString&)),
					win,SLOT(handleApplicationMessage(const QString &)));

	setActivationWindow(win, false);
	connect(win, SIGNAL(needToShow()), this, SLOT(activateWindow()));
}

Application::~Application()
{
	win->deleteLater();
}

