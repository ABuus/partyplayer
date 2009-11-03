#include "playlistview.h"

PlaylistView::PlaylistView(QWidget *parent)
	: QListView(parent)
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

    if(texts.first().isEmpty())
    {
		texts.clear();
		for(int i = 0; i < urls.count(); i++)
		{
			QString url = urls.at(i).toLocalFile();
		    url.remove("file://");
		    QByteArray ba = url.toLatin1();
			const char *file = ba.data();
		    Debug << file;
		    TagLib::FileRef f(file);

			if(!f.isNull() && f.tag())
		    {
				TagLib::Tag *tag = f.tag();
				QString title;
				title.append( QString::fromStdString( tag->artist().toCString() ));
				Debug << title;
				title.append( " - " );
				Debug << title;
				title.append( QString::fromStdString( tag->title().toCString() ));
				title.append( " - " );
				title.append( QString::fromStdString( tag->album().toCString() ));
				texts << title;
		    }
		}
    }
    Debug << texts;

    for(int i = 0; i < urls.count(); i++)
    {
		QStringList row;
		row << texts.at(i) << urls.at(i).toString();
		insetItem(row);
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


