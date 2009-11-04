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
    QStringList texts = event->mimeData()->text().split(";");

    Debug << urls.count() << texts.first().isEmpty();

	foreach(QUrl url, urls)
	{
		QList<QStandardItem *> row;
		Debug << url.scheme();
		if(url.scheme() == "file")
		{
			QString str = url.path();
#ifdef Q_WS_WIN
			str = str.right(1);
#endif
			QByteArray ba = str.toLatin1();
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
				row << artist << title << year << album << track << directory << length;
				model->appendRow(row);
		    }
			else
			{
				Debug << "Tag read error";
			}
		}
		else if(url.scheme() == "http")
		{

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
    QUrl url( model->itemFromIndex(index)->data(Qt::UserRole +1).toUrl());
    if(url.scheme() == "http")
    {
	emit playRequest(url,false);
    }
    else
    {
	emit playRequest(url,true);
    }
}


