#include "playlistcontextmenu.h"

using namespace Playlist;

PlaylistContextMenu::PlaylistContextMenu(QWidget *parent)
	: QMenu(parent),
	m_url()
{
	QAction *remove = new QAction(tr("Remove"),this);
	openExternal = new QAction(this);
	connect(remove,SIGNAL(triggered(bool)),this,SIGNAL(requestRemove()));
	connect(openExternal,SIGNAL(triggered(bool)),this,SLOT(openUrl()));
	menuActions.append(openExternal);
	menuActions.append(remove);

	foreach(QAction *a,menuActions)
	{
		addAction(a);
	}
}

PlaylistContextMenu::~PlaylistContextMenu()
{

}

void PlaylistContextMenu::exec(enum Playlist::InternalData location,const QUrl &url, const QPoint &pos)
{
	m_url = url;
	switch(location) {
		case Playlist::Local:
			openExternal->setText(tr("Open directory"));
			break;
		case Playlist::Youtube:
			openExternal->setText(tr("Open in browser"));
			break;
		default:
			return;
	}
	QMenu::popup(pos,menuActions.first());
}

void PlaylistContextMenu::openUrl()
{
	// online urls should be opend as is
	if(m_url.scheme() != QLatin1String("file"))
		QDesktopServices::openUrl(m_url);
	else
	{
		QString strUrl = m_url.toString();
		strUrl = strUrl.remove(strUrl.lastIndexOf("/"),strUrl.size());
		m_url = strUrl;
		QDesktopServices::openUrl(m_url);
	}
}
