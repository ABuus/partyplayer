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
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QListView>
#ifdef WEB_DEBUG
#include <QWebInspector>
#endif

// playlist
#include "../playlist/playlistview.h"
#include "../playlist/playlistmodel.h"
#include "../playlist/savedplaylistmodel.h"
// player
#include "../player/YoutubePlayer/youtubeplayer.h"
#ifdef Q_WS_WIN
#include "../player/QtDirectAudio/qtdirectaudio.h"
#else
#include "../player/GstPlayer/qgstplayer.h"
#endif
// self
#include "../debug.h"
#include "ui_mainwindow.h"
#include "youtubesearchview.h"
#include "youtubesearchmodel.h"
#include "controlwidget.h"

class MainWindow : public QMainWindow, public Ui::mainWindowClass
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();
	enum CurrentPlayer {
		Loacal = 1,
		Youtube
	};
public slots:
	bool handleApplicationMessage(const QString &);
private:
	Playlist::PlaylistView *m_playlist;
	Playlist::PlaylistModel *m_playlistModel;
	QFileSystemModel *fileSysModel;
	ControlWidget *controlWidget;
	YoutubeSearchView *youtubeSearchView;
	YoutubeSearchModel *youtubeSearchModel;
#ifdef Q_WS_WIN
	QtDirectAudio *localPlayer;
#else
	QGstPlayer *localPlayer;
#endif
	YoutubePlayer *youtubePlayer;
	YoutubeViewer *webView;
	QList<int> oldVSplitter;
	bool webState;
	void createConnections();
	void setCurrentPlayer(int current);
	int m_currentPlayer;
	QActionGroup *m_playerQualityActions;
	QListView *savedPlaylistView;
	Playlist::SavedPlaylistModel *savedPlaylistModel;
private slots:
	void queryYoutubeSearch();
	/*
	void clearSearch();
	void insertSearchItem(QStringList);
	*/
	void setVideoMode(QAction *);
	void playNextTrack();
	void playPreviousTrack();
	void handlePlayRequests(const QUrl url);
	void play();
	void pause();
	void stop();
	void seek(qint64 msec);
	void handlePlayerState(int state);
	void setTime(qint64);
	void setTotalTime(qint64);
	void setVideoQuality(QAction *);
	void openFileDialog();
	void openDirDialog();
signals:
	void preformSearch(QString text);
	void needToShow();
};

#endif // MAINWINDOW_H
