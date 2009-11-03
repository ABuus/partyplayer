#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);
	// youtube searcher
	search = new Search(this);
	searchModel = new SearchModel(this);
	searchView->setModel(searchModel);

	// control widget
	controlWidget = new ControlWidget(this);
	controlLayout->addWidget(controlWidget);
	
	// file system widget
	fileSysModel = new QFileSystemModel(this);
	fileView->setModel(fileSysModel);
	fileSysModel->setRootPath(QDir::rootPath());
	fileView->setCurrentIndex(fileSysModel->index(QDesktopServices::storageLocation(QDesktopServices::MusicLocation)));
	fileView->scrollTo(fileSysModel->index(QDesktopServices::storageLocation(QDesktopServices::MusicLocation)),QAbstractItemView::PositionAtTop);
	fileView->setColumnHidden(1,true);
	fileView->setColumnHidden(2,true);
	fileView->setColumnHidden(3,true);
	fileView->setColumnHidden(4,true);
	
	// temporary
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled,true);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled,true);
	
	// connections
	connect(this, SIGNAL( preformSearch( QString )), search, SLOT( query( QString )));
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT( querySearch()));
	connect(search, SIGNAL(newSearch()), this, SLOT(clearSearch()));
	connect(search, SIGNAL(newItem(QStringList)), this,SLOT(insertSearchItem(QStringList)));
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
	m_item->setData(item.at(2));
	searchModel->appendRow(m_item);
}