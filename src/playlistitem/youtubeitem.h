#ifndef YOUTUBEITEM_H
#define YOUTUBEITEM_H

#include <QObject>
#include "playlistitem_global.h"
#include "playlistitem.h"

namespace PlaylistItem {

class PLAYLISTITEM_EXPORT YoutubeItem : public PlaylistItem
{
	Q_OBJECT

public:
	YoutubeItem(const QString &url,QObject *parent);
	YoutubeItem(const QUrl &url,QObject *parent);
	~YoutubeItem();
	QString artist() { return m_artist; };
	QString comments() { return m_comments; };
	uint length() { return m_length; };
	QPixmap image() { return m_image; };
private:
	QString m_artist;
	QString m_comments;
	uint m_length;
	QPixmap m_image;
};

}; // namespace PlaylistItem

#endif // YOUTUBEITEM_H
