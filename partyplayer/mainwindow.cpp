#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	// this
	setupUi(this);

	// youtube searcher
	search = new Search(this);
	searchModel = new SearchModel(this);
	searchView->setModel(searchModel);

	// player
	player = new Player(this);

	// control widget
	controlWidget = new ControlWidget(this);
	controlLayout->addWidget(controlWidget);
	controlWidget->seekSlider->setMediaObject( player->mediaObject() );
	
	// file system widget
	fileSysModel = new QFileSystemModel(this);
	fileView->setModel(fileSysModel);
	fileView->setColumnHidden(1,true);
	fileView->setColumnHidden(2,true);
	fileView->setColumnHidden(3,true);
	fileView->setColumnHidden(4,true);
	fileSysModel->setRootPath(QDir::rootPath());
	QModelIndex musicIndex = fileSysModel->index(QDesktopServices::storageLocation(QDesktopServices::MusicLocation));	
	fileView->expand(musicIndex);
	fileView->setCurrentIndex(musicIndex);
	fileView->scrollTo(musicIndex,QAbstractItemView::PositionAtCenter);
	
	// websettings to ensure we can play youtube vids
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled,true);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
	
	// connections
	connect(this, SIGNAL( preformSearch( QString )), search, SLOT( query( QString )));
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT( querySearch()));
	connect(search, SIGNAL(newSearch()), this, SLOT(clearSearch()));
	connect(search, SIGNAL(newItem(QStringList)), this,SLOT(insertSearchItem(QStringList)));
	connect(playlistView,SIGNAL(playRequest(QUrl,bool)),player,SLOT(play(QUrl,bool)));
	connect(playlistView,SIGNAL(nextRequest(QUrl,bool)),player,SLOT(setNext(QUrl,bool)));
	connect(player,SIGNAL(requestNext()),playlistView,SLOT(findNext()));
	connect(player,SIGNAL(currentSourceChanged(const QUrl &)),playlistView,SLOT(checkCurrentIndex(const QUrl &)));
	connect(controlWidget,SIGNAL(stop()),player,SLOT(stop()));
	connect(controlWidget,SIGNAL(play()),player,SLOT(play()));
	connect(controlWidget,SIGNAL(pause()),player,SLOT(pause()));
	connect(controlWidget,SIGNAL(back()),player,SLOT(previous()));
	connect(controlWidget,SIGNAL(forward()),player,SLOT(next()));
}

mainwindow::~mainwindow()
{
	
}

void mainwindow::querySearch()
{
	emit preformSearch( searchLineEdit->text() );
}

void mainwindow::clearSearch()
{
	searchModel->clear();
}

void mainwindow::insertSearchItem(QStringList item)
{
	QStandardItem *m_item = new QStandardItem(item.first());
	m_item->setData(item.at(1),Qt::ToolTipRole);
	m_item->setData(QUrl(item.at(2)), Qt::UserRole +1);
	Debug << item.at(2);
	searchModel->appendRow(m_item);
}
