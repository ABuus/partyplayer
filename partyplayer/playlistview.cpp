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
	Debug << "insert count" << urls.count();
	foreach(QUrl url, urls)
	{
		QList<QStandardItem *> row;
		Debug << url;
		// create addFile(QUrl url)
		if(url.scheme() == "file")
		{
			QString str = url.path();
			QByteArray ba = str.toLatin1();
#ifdef Q_WS_WIN // remove "/" in "/C:\..."
			ba = ba.right(ba.size() -1);
#endif
			const char *file = ba.data();
			Debug << file;
			TagLib::FileRef f(file);
			
			if(!f.isNull() && f.tag())
		    {
				TagLib::Tag *tag = f.tag();
				TagLib::AudioProperties *ap = f.audioProperties();
				QStandardItem *artist = new QStandardItem( QString::fromStdString( tag->artist().toCString() ));
				QStandardItem *title = new QStandardItem( QString::fromStdString( tag->title().toCString() ));
				QStandardItem *year = new QStandardItem( QString::number( tag->year() ));
				QStandardItem *album = new QStandardItem( QString::fromStdString( tag->album().toCString() ));
				QStandardItem *track = new QStandardItem( QString::number(tag->track()) );
				QStandardItem *directory = new QStandardItem( url.path() );
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
				artist->setData(url,Qt::UserRole +1);
				row << artist << title << year << album << track << directory << length;
				model->appendRow(row);
		    }
			else
			{
				// foreach() addFile()
				Debug << "Tag read error";
				QStandardItem *artist = new QStandardItem( "Tag read error" );
				artist->setData(url,Qt::UserRole +1);
				row << artist;
				model->appendRow(row);
			}
		}
		// addLink()
		else if(url.scheme() == "http")
		{
			QStringList strList = event->mimeData()->text().split(":");
			QStandardItem *artist = new QStandardItem(strList.takeFirst());
			QStandardItem *title = new QStandardItem( QString() );
			QStandardItem *year = new QStandardItem( QString() );
			QStandardItem *album = new QStandardItem( QString());
			QStandardItem *track = new QStandardItem( QString() );
			QStandardItem *directory = new QStandardItem( "YouTube" );

			const int time = strList.first().toInt();
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
			row << artist << title << year << album << track << directory << length;
			model->appendRow(row);
		}
		else
		{
			Debug << "unsupported content:" << url;
		}
	}
}

void PlaylistView::insetItem(QStringList item)
{
    QStandardItem *m_item = new QStandardItem(item.first());
    m_item->setData(QUrl(item.at(1)), Qt::UserRole +1);
    Debug << m_item->data(Qt::UserRole +1);
    model->appendRow(m_item);
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