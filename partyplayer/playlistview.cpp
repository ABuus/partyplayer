#include "playlistview.h"

PlaylistView::PlaylistView(QWidget *parent)
	: QTableView(parent)
{
	model = new PlaylistModel(this);
	setModel(model);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
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
    Debug << "start";
    event->acceptProposedAction();
    const QList<QUrl> urls = event->mimeData()->urls();
    
	foreach(QUrl url, urls)
	{
		QList<QStandardItem *> row;
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
				QStandardItem *artist = new QStandardItem( QString::fromStdString( tag->artist().toCString() ));
				QStandardItem *title = new QStandardItem( QString::fromStdString( tag->title().toCString() ));
				QStandardItem *year = new QStandardItem( tag->year() );
				QStandardItem *album = new QStandardItem( QString::fromStdString( tag->album().toCString() ));
				QStandardItem *track = new QStandardItem( tag->track() );
				QStandardItem *directory = new QStandardItem( url.path() );
				QStandardItem *length = new QStandardItem( QString::fromStdString( tag->album().toCString() ));
				artist->setData(url,Qt::UserRole +1);
				row << artist << title << year << album << track << directory << length;
				model->appendRow(row);
				return;
		    }
			else
			{
				Debug << "Tag read error";
				return;
			}
		}
		else if(url.scheme() == "http")
		{
			QStandardItem *title = new QStandardItem(event->mimeData()->text());
			title->setData(url, Qt::UserRole +1);
			row << title;
			model->appendRow(row);
			return;
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
    if(url.scheme() == "http")
    {
		emit playRequest(url,false);
    }
    else
    {
		Debug << "emitting" << url;
		emit playRequest(url,true);
    }
}

void PlaylistView::findNext()
{
	QModelIndex index = currentIndex();
	if(!index.isValid())
		return;
	QModelIndex nextIndex = model->index(index.row() +1,0);

	QUrl url( nextIndex.data(Qt::UserRole +1).toUrl());
	if(url.scheme() == "http")
    {
		emit nextRequest(url,false);
    }
    else
    {
		Debug << "emitting" << url;
		emit nextRequest(url,true);
    }
}

void PlaylistView::checkCurrentIndex(const QUrl &url)
{
	QModelIndex row = currentIndex();
	QModelIndex index = model->index(row.row(),0);
	if(!index.isValid())
		return;
	int i = url.toString().compare( index.data(Qt::UserRole +1).toString(), Qt::CaseInsensitive);
	Debug << i << url.toString() << index.data(Qt::UserRole +1).toString();
	if(i == 0)
		return;
	
	QModelIndex nextIndex = model->index(index.row() +1,0);
	if(!nextIndex.isValid())
		return;
	setCurrentIndex(nextIndex);
}