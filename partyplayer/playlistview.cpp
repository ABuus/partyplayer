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
	event->acceptProposedAction();
	QList<QUrl> urls = event->mimeData()->urls();
	QStringList texts = event->mimeData()->text().split(";");
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
    Debug << item.at(1);
    model->appendRow(m_item);
}
