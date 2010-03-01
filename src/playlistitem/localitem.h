#ifndef LOCALITEM_H
#define LOCALITEM_H

#include "playlistitem_global.h"
#include "playlistitem.h"
#include "../debug.h"

namespace PlaylistItem {

class PLAYLISTITEM_EXPORT LocalItem : public PlaylistItem
{
	Q_OBJECT

public:
	LocalItem(const QString &url,QObject *parent = 0);
	LocalItem(const QUrl &url,QObject *parent = 0);
	~LocalItem();
	QString artist() const { return m_artist; };
	QString title() const { return m_title; };
	QString album() const { return m_album; };
	QString genre() const { return m_genre; };
	QString comment() const { return m_comment; };
	uint track() const { return m_track; };
	uint year() const { return m_year; };
	uint length() const { return m_length; };
	uint bitrate() const { return m_bitrate; };
	uint channels() const { return m_channels; };
	uint samplerate() const { return m_samplerate; };
	QPixmap image() const { return m_image; };
private:
	void init();
	void readMpegImage();
	bool readDefault();
	QString m_artist;
	QString m_title;
	QString m_album;
	QString m_genre;
	QString m_comment;
	uint m_track;
	uint m_year;
	uint m_length;
	uint m_bitrate;
	uint m_channels;
	uint m_samplerate;
	QPixmap m_image;
};

}; // namespace PlaylistItem

#endif // LOCALITEM_H
