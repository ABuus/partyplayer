#include "mainwindow.h"
#include <QtGui/QApplication>
#include "debug.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("PartyPlayer");
	a.setOrganizationName("BuusSW");
//	QString style(loadStyle());
//	a.setStyleSheet(style);
	MainWindow w;
	w.show();
	return a.exec();
}

