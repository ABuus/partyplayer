#include "playlistview.h"

PlaylistView::PlaylistView(QWidget *parent)
	: QTableView(parent)
{
	model = new PlaylistModel(this);
	setModel(model);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode( QAbstractItemView::DragDrop );
}

PlaylistView::~PlaylistView()
{

}

void PlaylistView::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
}

void PlaylistView::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    const QList<QUrl> urls = event->mimeData()->urls();
	foreach(QUrl url, urls)
	{
		Debug << url.scheme();
		if(url.scheme() == "file")
		{
			insertFile(url.toString(),rowAt(event->pos().y()));   
		}
		else if(url.scheme() == "http")
		{
			QStringList strList = event->mimeData()->text().split(":");
			const QString title = strList.first();
			const int time = strList.at(1).toInt();
			int row = rowAt(event->pos().y());
			insertYTItem(url,title,time,row);
		}
		else
		{
			Debug << "unsupported content:" << url;
		}
	}
}


void PlaylistView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if(!index.isValid())
		return;
	QUrl url( model->index(index.row(),0).data(Qt::UserRole +1).toUrl());
	emit playRequest(url);
}

QUrl PlaylistView::next()
{
	QModelIndex index = currentIndex();
	if(!index.isValid())
		return QUrl();
	QModelIndex nextIndex = model->index(index.row() +1,0);
	if(!nextIndex.isValid())
		return QUrl();
	QUrl url( nextIndex.data(Qt::UserRole +1).toUrl());
	return url;
}

QUrl PlaylistView::previous()
{
	QModelIndex index = currentIndex();
	QModelIndex previousIndex = model->index(index.row() -1,0);
	QUrl url( previousIndex.data(Qt::UserRole +1).toUrl());
	return url;
}

void PlaylistView::selectNext()
{
	QModelIndex index = currentIndex();
	if(!index.isValid())
		return;
	QModelIndex nextIndex = model->index(index.row() +1,0);
	if(!nextIndex.isValid())
		return;
	setCurrentIndex(nextIndex);
}

void PlaylistView::selectPrevious()
{
	QModelIndex index = currentIndex();
	if(!index.isValid())
		return;
	QModelIndex nextIndex = model->index(index.row() -1,0);
	if(!nextIndex.isValid())
		return;
	setCurrentIndex(nextIndex);
}

void PlaylistView::insertYTItem(const QUrl url, const QString title, const int time, int row)
{
	if(row == -1)
	{
		row = model->rowCount();
	}
	QList<QStandardItem *> rowItem;
	QStandardItem *artist = new QStandardItem(title);
	QStandardItem *m_title = new QStandardItem( QString() );
	QStandardItem *year = new QStandardItem( QString() );
	QStandardItem *album = new QStandardItem( QString());
	QStandardItem *track = new QStandardItem( QString() );
	QStandardItem *directory = new QStandardItem( "YouTube" );

	const int min = time / 60;
	const int sec = time % 60;
	QString strSec = QString::number(sec);
	if(sec < 10)
	{
		strSec.prepend("0");
	}
	QString timeStr( QString::number(min) + ":" + strSec );
	QStandardItem *length = new QStandardItem( timeStr );

	artist->setData(url, Qt::UserRole +1);
	artist->setData(time, Qt::UserRole +2);
	rowItem << artist << m_title << year << album << track << directory << length;
	model->insertRow(row,rowItem);
}

void PlaylistView::insertFile(const QString file, int row)
{
	if(row == -1)
	{
		row = model->rowCount();
	}

	QByteArray ba = file.toLatin1();
	if(ba.startsWith("file:///"))
		ba.remove(0,7);	
#ifdef Q_WS_WIN // remove "/" in "/C:\..."
	if(ba.startsWith("/"))
		ba = ba.right(ba.size() -1);
#endif

	const char *tFile = ba.data();

	Debug << file << tFile;
	QList<QStandardItem *> rowItem;
	TagLib::FileRef f(tFile);
	if(!f.isNull() && f.tag())
    {
		TagLib::Tag *tag = f.tag();
		TagLib::AudioProperties *ap = f.audioProperties();
		QStandardItem *artist = new QStandardItem( QString::fromStdString( tag->artist().toCString() ));
		QStandardItem *title = new QStandardItem( QString::fromStdString( tag->title().toCString() ));
		QStandardItem *year = new QStandardItem( QString::number( tag->year() ));
		QStandardItem *album = new QStandardItem( QString::fromStdString( tag->album().toCString() ));
		QStandardItem *track = new QStandardItem( QString::number(tag->track()) );
		QStandardItem *directory = new QStandardItem( file );
		const int time = ap->length();
		const int min = time / 60;
		const int sec = time % 60;
		QString strSec = QString::number(sec);
		if(sec < 10)
		{
			strSec.prepend("0");
		}
		QString timeStr( QString::number(min) + ":" + strSec );
		QStandardItem *length = new QStandardItem( timeStr );
		artist->setData(file,Qt::UserRole +1);
		rowItem << artist << title << year << album << track << directory << length;
		model->insertRow(row,rowItem);
	}
	else
	{
		QFileInfo fi(tFile);
		if(fi.isDir())
		{
			Debug << "dir insert";
			insertDir(QUrl(file),row);
		}
		else
			Debug << "unsuported content";
	}
}

void PlaylistView::insertDir(const QUrl url, int row)
{
	QFileInfo info(url.toLocalFile());
	if(!info.isDir())
	{
		Debug << "error not a directory";
		return;
	}
	QDir dir(url.toLocalFile());
	QFileInfoList files = dir.entryInfoList(QDir::Files);
	foreach(QFileInfo fi,files)
	{
		insertFile(fi.absoluteFilePath(),row);
		row++;	
	}
}