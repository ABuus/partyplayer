#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	webState(false)
{
	// this
	QSettings settings(QApplication::organizationName(), QApplication::applicationName());
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

	webView = new QWebView(this);
	// websettings to ensure we can play youtube vids
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled,true);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
	webView->show();
	webView->load(QUrl("http://www.youtube.com/apiplayer?version=3"));
//	player->setWebView(webView);
	videoContainer->addWidget(webView);

	// control widget
	controlWidget = new ControlWidget(this);
	controlLayout->addWidget(controlWidget);
//	controlWidget->seekSlider->setMediaObject( player->mediaObject() );

	// file system widget
	fileSysModel = new QFileSystemModel(this);
	fileView->setModel(fileSysModel);
	fileView->setColumnHidden(1,true);
	fileView->setColumnHidden(2,true);
	fileView->setColumnHidden(3,true);
	fileView->setColumnHidden(4,true);
	fileSysModel->setRootPath(QDir::rootPath());

	// this does not seem to work
	QModelIndex musicIndex = fileSysModel->index(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));	
	fileView->expand(musicIndex);
	fileView->setCurrentIndex(musicIndex);
	fileView->scrollTo(musicIndex,QAbstractItemView::PositionAtCenter);
	
	player = new Player();

	// connections
	connect(this, SIGNAL( preformSearch( QString )), search, SLOT( query( QString )));
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT( querySearch()));
	connect(search, SIGNAL(newSearch()), this, SLOT(clearSearch()));
	connect(search, SIGNAL(newItem(QStringList)), this,SLOT(insertSearchItem(QStringList)));
	connect(menuMode,SIGNAL(triggered(QAction *)),this,SLOT(setVideoMode(QAction *)));
	connect(m_playlist,SIGNAL(playRequest(const QUrl &)),this,SLOT(handlePlayRequests(const QUrl &)));
	connect(player,SIGNAL(timeChanged(qint64)),controlWidget,SLOT(setTime(qint64)));
	connect(player,SIGNAL(totalTimeChanged(qint64)),controlWidget,SLOT(setTotalTime(qint64)));
	connect(controlWidget,SIGNAL(seek(int)),player,SLOT(seek(int)));
	connect(controlWidget,SIGNAL(stop()),player,SLOT(stop()));
	connect(controlWidget,SIGNAL(play()),player,SLOT(play()));
	connect(controlWidget,SIGNAL(pause()),player,SLOT(pause()));
	connect(player,SIGNAL(runningOut()),this,SLOT(enqueueNextTrack()));
	connect(controlWidget,SIGNAL(forward()),this,SLOT(playNextTrack()));
	connect(controlWidget,SIGNAL(back()),this,SLOT(playPreviousTrack()));

//	connect(player,SIGNAL(playStateChanged(bool)),controlWidget,SLOT(setPlayState(bool)));
}

MainWindow::~MainWindow()
{
	player->deleteLater();
	QSettings settings(QApplication::organizationName(), QApplication::applicationName());
	settings.setValue("mainwindow/geometry",saveGeometry());
	settings.setValue("mainwindow/windowState", saveState());
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
	m_item->setData(QUrl(item.at(2)), Qt::UserRole +1);
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

void MainWindow::enqueueNextTrack()
{
	QUrl url = m_playlist->next();
	if(url.scheme() == "http")
	{
		Debug << "loading http";
		webView->load(url);
		webState = true;
		player->stop();
	}
	else
	{
		Debug << "loading file";
		if(webState)
			webView->load(QUrl("http://www.youtube.com/apiplayer?version=3"));
		player->enqueue(url);
	}
}

void MainWindow::playNextTrack()
{
	QUrl url = m_playlist->next();
	if(url.scheme() == "http")
	{
		Debug << "loading http";
		webView->load(url);
		webState = true;
		player->stop();
	}
	else
	{
		Debug << "loading file";
		if(webState)
			webView->load(QUrl("http://www.youtube.com/apiplayer?version=3"));
		player->playUrl(url);
	}
}

void MainWindow::playPreviousTrack()
{
	QUrl url = m_playlist->previous();
	if(url.scheme() == "http")
	{
		Debug << "loading http";
		webView->load(url);
		webState = true;
		player->stop();
	}
	else
	{
		Debug << "loading file";
		if(webState)
			webView->load(QUrl("http://www.youtube.com/apiplayer?version=3"));
		player->playUrl(url);
	}
}

void MainWindow::handlePlayRequests(const QUrl &url)
{
	Debug << url;
	if(url.scheme() == "http")
	{
		Debug << "loading http";
		if(actionYouTube_HD->isChecked())
		{
			QString str = url.toString();
			str.append("&hd=1");
			webView->load(QUrl(str));
		}
		else
		{
			webView->load(url);
		}
		webState = true;
		player->stop();
	}
	else
	{
		Debug << "loading file";
		if(webState)
			webView->load(QUrl("http://www.youtube.com/apiplayer?version=3"));
		player->playUrl(url);
	}
}