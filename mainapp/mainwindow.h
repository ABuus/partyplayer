#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QtGui/QMainWindow>
#include <QApplication>
#include <QSettings>
#include <QObject>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QWebView>
#include <QAction>
// playlist
#include "playlist/playlistview.h"
// player
#include "player/player.h"
// self
#include "debug.h"
#include "ui_mainwindow.h"
#include "search.h"
#include "searchmodel.h"
#include "controlwidget.h"

class MainWindow : public QMainWindow, public Ui::mainWindowClass
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();
private:
	Playlist::PlaylistView *m_playlist;
	Search *search;
	QFileSystemModel *fileSysModel;
	ControlWidget *controlWidget;
	SearchModel *searchModel;
	Player *player;
	QWebView *webView;
	QList<int> oldVSplitter;
private slots:
	void querySearch();
	void clearSearch();
	void insertSearchItem(QStringList);
	void setVideoMode(QAction *);
	void fetchNextTrack();
signals:
	void preformSearch(QString text);
};

#endif // MAINWINDOW_H
