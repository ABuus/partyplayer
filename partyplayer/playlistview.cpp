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
	for(int i = 0; i < urls.count(); i++)
	{
		Debug << urls.at(i).toString();
//		model->addUrl(urls.at(i));
	}
}