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

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	webState(false),
	m_currentPlayer(0)
{
	// restore geometry and states
	QSettings settings(QApplication::organizationName(), QApplication::applicationName());
	settings.setDefaultFormat(QSettings::IniFormat);
	restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
	restoreState(settings.value("mainwindow/windowState").toByteArray());
	setupUi(this);

	// youtube searcher
	search = new Search(this);
	searchModel = new SearchModel(this);
	searchView->setModel(searchModel);
	
	//playlist
	m_playlist = new Playlist::PlaylistView(this);
	playlistContainer->addWidget(m_playlist);
	m_playlist->show();

	// local player
	localPlayer = new LocalPlayer(this);

	// youtube player
	webView = new YoutubeViewer(this); // in youtubeplayer.h
#ifdef Q_WS_X11
	webView->setFixedSize(500,400);
#endif
	youtubePlayer = new YoutubePlayer(webView);
	webView->setPage(youtubePlayer);
	videoContainer->addWidget(webView);
#ifdef WEB_DEBUG
	QWebInspector *in = new QWebInspector;
	in->setPage(youtubePlayer);
	in->show();
#endif

	// control widget
	controlWidget = new ControlWidget(this);
	controlLayout->addWidget(controlWidget);

	// file system widget
	// due to bug in QFileSystemModel windows shares have to be mapped as drives
	// http://bugreports.qt.nokia.com/browse/QTBUG-4462
	fileSysModel = new QFileSystemModel(this);
	fileSysModel->setRootPath("/");
	fileView->setModel(fileSysModel);
	fileView->setColumnHidden(1,true);
	fileView->setColumnHidden(2,true);
	fileView->setColumnHidden(3,true);
	fileView->setColumnHidden(4,true);
	
	// this does not seem to work on linux (opensuse 11.2)
	QModelIndex musicIndex = fileSysModel->index(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));	
	fileView->expand(musicIndex);
	fileView->setCurrentIndex(musicIndex);
	fileView->scrollTo(musicIndex,QAbstractItemView::PositionAtCenter);

	/*
	webView->restoreGeometry(settings.value("mainwindow/webView").toByteArray());
	m_playlist->restoreGeometry(settings.value("mainwindow/playlist").toByteArray());
	videoSplitter->restoreGeometry(settings.value("mainwindow/videoSplitter").toByteArray());
	*/
	QList<int> sizes;
	sizes << 0 << 1;
	videoSplitter->setSizes(sizes);

	m_playerQualityActions = new QActionGroup(this);
	m_playerQualityActions->addAction(ytQualitySmall);
	m_playerQualityActions->addAction(ytQualityMedium);
	m_playerQualityActions->addAction(ytQualityHigh);
	m_playerQualityActions->addAction(ytQualityHd_720);
	m_playerQualityActions->addAction(ytQualityStandard);
	m_playerQualityActions->setExclusive(true);

	createConnections();
}

MainWindow::~MainWindow()
{
	QSettings settings(QApplication::organizationName(), QApplication::applicationName());
	settings.setDefaultFormat(QSettings::IniFormat);
	settings.setValue("mainwindow/geometry",saveGeometry());
	settings.setValue("mainwindow/windowState", saveState());
	settings.setValue("mainwindow/playlist", videoSplitter->saveGeometry());
	settings.setValue("mainwindow/webView", videoSplitter->saveGeometry());
	settings.setValue("mainwindow/videoSplitter", videoSplitter->saveGeometry());
}

void MainWindow::createConnections()
{
	// youtube search
	connect(this, SIGNAL( preformSearch( QString )), search, SLOT( query( QString )));
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT( querySearch()));
	connect(search, SIGNAL(newSearch()), this, SLOT(clearSearch()));
	connect(search, SIGNAL(newItem(QStringList)), this,SLOT(insertSearchItem(QStringList)));

	// player control
	connect(controlWidget,SIGNAL(forward()),this,SLOT(playNextTrack()));
	connect(controlWidget,SIGNAL(back()),this,SLOT(playPreviousTrack()));
	connect(controlWidget,SIGNAL(seek(int)),this,SLOT(seek(int)));
	connect(controlWidget,SIGNAL(stop()),this,SLOT(stop()));
	connect(controlWidget,SIGNAL(play()),this,SLOT(play()));
	connect(controlWidget,SIGNAL(pause()),this,SLOT(pause()));
	connect(m_playlist,SIGNAL(playRequest(const QVariant &)),this,SLOT(handlePlayRequests(const QVariant &)));
	connect(localPlayer,SIGNAL(runningOut()),this,SLOT(enqueueNextTrack()));
	connect(youtubePlayer,SIGNAL(finished()),this,SLOT(playNextTrack()));
	connect(localPlayer,SIGNAL(stateChanged(int)),this,SLOT(handlePlayerState(int)));
	connect(youtubePlayer,SIGNAL(stateChanged(int)),this,SLOT(handlePlayerState(int)));
	connect(localPlayer,SIGNAL(timeChanged(qint64)),this,SLOT(setTime(qint64)));
	connect(youtubePlayer,SIGNAL(currentTimeChanged(qint64)),this,SLOT(setTime(qint64)));
	connect(localPlayer,SIGNAL(totalTimeChanged(qint64)),this,SLOT(setTotalTime(qint64)));
	connect(youtubePlayer,SIGNAL(totalTimeChanged(qint64)),this,SLOT(setTotalTime(qint64)));
	
	// actions & menus
	connect(actionClearPlaylist,SIGNAL(triggered()),m_playlist,SLOT(clear()));
	connect(actionSavePlaylist,SIGNAL(triggered()),m_playlist,SLOT(save()));
	connect(menuMode,SIGNAL(triggered(QAction *)),this,SLOT(setVideoMode(QAction *)));
	connect(ytPbQuality,SIGNAL(triggered(QAction *)),this,SLOT(setVideoQuality(QAction *)));
	connect(actionOpenFile,SIGNAL(triggered(bool)),this,SLOT(openFileDialog()));
	connect(actionOpenDir,SIGNAL(triggered(bool)),this,SLOT(openDirDialog()));
}

void MainWindow::setCurrentPlayer(int player)
{
	if(m_currentPlayer == player)
		return;
	else
		m_currentPlayer = player;
}

void MainWindow::querySearch()
{
	emit preformSearch( searchLineEdit->text() );
}

void MainWindow::clearSearch()
{
	searchModel->clear();
}

void MainWindow::insertSearchItem(QStringList item)
{
	QStandardItem *m_item = new QStandardItem(item.first());
	m_item->setData(item.at(1),Qt::ToolTipRole);
	m_item->setData(item.at(2), Qt::UserRole +1);
	m_item->setData(item.at(3), Qt::UserRole +2);
	Debug << item.at(2);
	searchModel->appendRow(m_item);
}

void MainWindow::setVideoMode(QAction *a)
{
	if(a->objectName() == "Audio_Only")
	{
		oldVSplitter = videoSplitter->sizes();
		webView->hide();
		QList<int> i;
		i << 0 << ( oldVSplitter.at(0) + oldVSplitter.at(1) );
		videoSplitter->setSizes(i);
		a->setChecked(true);
	}
	else if(a->objectName() == "Video")
	{
		if(oldVSplitter.isEmpty())
		{
			int h = ( videoSplitter->sizes().at(0) + videoSplitter->sizes().at(1) ) / 2;
			Debug << h;
			oldVSplitter << h << h;
		}
		videoSplitter->setSizes(oldVSplitter);
		webView->show();
		a->setChecked(true);
	}
}


/* todo enqueue in ytplayer */
void MainWindow::enqueueNextTrack()
{
	QString value = m_playlist->next().toString();
	if(value.isEmpty())
		return;
	if(value.startsWith("file", Qt::CaseInsensitive))
	{
		setCurrentPlayer(MainWindow::Loacal);
		if(webState)
		{
			webState = false;
			youtubePlayer->pause();
		}
		localPlayer->enqueue(QUrl(value));
	}
	else
	{
		setCurrentPlayer(MainWindow::Youtube);
		/* enqueueVideoBuId ??? */
		youtubePlayer->loadVideoById(value);
		webState = true;
		localPlayer->stop();
	}
}

void MainWindow::playNextTrack()
{
	QString value = m_playlist->next().toString();
	if(value.isEmpty())
		return;
	if(value.startsWith("file", Qt::CaseInsensitive))
	{
		setCurrentPlayer(MainWindow::Loacal);
		if(webState)
		{
			webState = false;
			youtubePlayer->pause();
		}
		localPlayer->playUrl(QUrl(value));
	}
	else
	{
		setCurrentPlayer(MainWindow::Youtube);
		youtubePlayer->loadVideoById(value);
		webState = true;
		localPlayer->stop();

	}
}

void MainWindow::playPreviousTrack()
{
	QString value = m_playlist->previous().toString();
	if(value.isEmpty())
		return;
	if(value.startsWith("file", Qt::CaseInsensitive))
	{
		setCurrentPlayer(MainWindow::Loacal);
		if(webState)
		{
			webState = false;
			youtubePlayer->pause();
		}
		localPlayer->enqueue(QUrl(value));
	}
	else
	{
		setCurrentPlayer(MainWindow::Youtube);
		youtubePlayer->loadVideoById(value);
		webState = true;
		localPlayer->stop();
	}
}

void MainWindow::handlePlayRequests(const QVariant &req)
{
	QString value = req.toString();
	qDebug() << "play request" << value;
	if( value.startsWith("file", Qt::CaseInsensitive))
	{
		setCurrentPlayer(MainWindow::Loacal);
		if(webState)
		{
			webState = false;
			youtubePlayer->pause();
		}
		localPlayer->playUrl(QUrl(value));
	}
	else
	{
		setCurrentPlayer(MainWindow::Youtube);
		youtubePlayer->loadVideoById(value);
		webState = true;
		localPlayer->stop();
	}
}

/* handles incomming messeges from QSingleApplication see Application.cpp ctor */

bool MainWindow::handleApplicationMessage(const QString &msg)
{
	qDebug() << "message recived by new App instance" << msg;
	QStringList files = msg.split("* *");
	files.removeFirst();
	int row = -1;
	foreach(QString file, files)
	{
		file.prepend("file:///");
		m_playlist->addFile(file,row++);
	}
	emit needToShow();
	return true;
}

void MainWindow::play()
{
	if(m_currentPlayer == MainWindow::Loacal)
		localPlayer->play();
	else if(m_currentPlayer == MainWindow::Youtube)
		youtubePlayer->play();
	else
		return;
}

void MainWindow::pause()
{
	if(m_currentPlayer == MainWindow::Loacal)
		localPlayer->pause();
	else if(m_currentPlayer == MainWindow::Youtube)
		youtubePlayer->pause();
	else
		return;
}

void MainWindow::stop()
{	
	if(m_currentPlayer == MainWindow::Loacal)
		localPlayer->stop();
	else if(m_currentPlayer == MainWindow::Youtube)
		youtubePlayer->pause();
	else
		return;
}

void MainWindow::seek(int msec)
{
	if(m_currentPlayer == MainWindow::Loacal)
		localPlayer->seek(msec);
	else if(m_currentPlayer == MainWindow::Youtube)
		youtubePlayer->seek(msec);
	else
		return;
}

void MainWindow::handlePlayerState(int state)
{
	QObject *player = qobject_cast<QObject *>( sender() );
	if(player == localPlayer && m_currentPlayer == MainWindow::Loacal)
	{
		controlWidget->setPlayState(state);
	}
	else if(player == youtubePlayer && m_currentPlayer == MainWindow::Youtube)
	{
		controlWidget->setPlayState(state);
	}
	else return;
}

void MainWindow::setTime(qint64 time)
{
	QObject *player = qobject_cast<QObject *>( sender() );
	if(player == localPlayer && m_currentPlayer == MainWindow::Loacal)
	{
		controlWidget->setTime(time);
	}
	else if(player == youtubePlayer && m_currentPlayer == MainWindow::Youtube)
	{
		controlWidget->setTime(time);
	}
	else return;
}

void MainWindow::setTotalTime(qint64 time)
{
	QObject *player = qobject_cast<QObject *>( sender() );
	if(player == localPlayer && m_currentPlayer == MainWindow::Loacal)
	{
		controlWidget->setTotalTime(time);
	}
	else if(player == youtubePlayer && m_currentPlayer == MainWindow::Youtube)
	{
		controlWidget->setTotalTime(time);
	}
	else return;
}

void MainWindow::setVideoQuality(QAction *a)
{
	int quality = -1;
	if(a->objectName() == "ytQualitySmall")
		quality = YoutubePlayer::Low;
	else if(a->objectName() == "ytQualityMedium")
		quality = YoutubePlayer::Medium;
	else if(a->objectName() == "ytQualityHigh")
		quality = YoutubePlayer::High;
	else if(a->objectName() == "ytQualityHd_720")
		quality = YoutubePlayer::Hd720;
	else if(a->objectName() == "ytQualityStandard")
		quality = YoutubePlayer::Standard;
	else
		return;
	youtubePlayer->setPlayQuality(quality);
}

void MainWindow::openDirDialog()
{
	QString dir = QFileDialog::getExistingDirectory(this,tr("Open Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly);
	m_playlist->addFile(dir);
}

void MainWindow::openFileDialog()
{
	QStringList files = QFileDialog::getOpenFileNames(this,"*",QDir::homePath());
	foreach(QString file, files)
	{
		m_playlist->addFile(file);
	}
}
