#include "mainwindow.h"
#include <QtGui/QApplication>

QString loadStyle();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("PartyPlayer");
	a.setOrganizationName("BuusSW");
	QString style(loadStyle());
	a.setStyleSheet(style);
	MainWindow w;
	w.show();
	return a.exec();
}

QString loadStyle()
{
	QString style;
	style.append("QToolButton {width:60px; height:60px; border:none;}");
	style.append("PlaylistView { selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \n\tstop: 0 #D3D3D3, stop: 0.4 #D8D8D8, \n\tstop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);}");
	style.append("PlaylistView { selection-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \n\tstop: 0 #C3C3C3, stop: 0.4 #C1C1C1, \n\tstop: 0.5 #C5C5C5, stop: 1.0 #D1D1D1);}");
	style.append("PlaylistView::item { height: 4px;}");
	return style;
}

