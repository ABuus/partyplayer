#include "youtubeitem.h"

namespace PlaylistItem {

YoutubeItem::YoutubeItem(const QString &url,QObject *parent)
	: PlaylistItem(url,parent)
{

}

YoutubeItem::YoutubeItem(const QUrl &url,QObject *parent)
	: PlaylistItem(url,parent)
{

}

YoutubeItem::~YoutubeItem()
{

}

};