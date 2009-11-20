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
	bool webState;
private slots:
	void querySearch();
	void clearSearch();
	void insertSearchItem(QStringList);
	void setVideoMode(QAction *);
	void enqueueNextTrack();
	void playNextTrack();
	void playPreviousTrack();
	void handlePlayRequests(const QUrl &url);
signals:
	void preformSearch(QString text);
};

#endif // MAINWINDOW_H
