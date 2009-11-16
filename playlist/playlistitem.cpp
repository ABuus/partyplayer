#include "playlistitem.h"

PlaylistItem::PlaylistItem(const QUrl url)
{
	QList<QStandardItem *> row;
	QString file;
	if(url.scheme() == "file")
		file = url.toLocalFile();
	QByteArray ba(file.toLatin1());
	const char *tFile = ba.data();
	TagLib::FileRef f(tFile);
	if(!f.isNull() && f.tag())
    {
		TagLib::Tag *tag = f.tag();
		TagLib::AudioProperties *ap = f.audioProperties();
		m_artist = tag->artist().toCString();
		m_title = tag->title().toCString();
		m_album = tag->album().toCString();
		m_place = file;
		m_year = tag->year();
		m_track = tag->track();
		m_length = ap->length();
		m_bitrate = ap->bitrate();
		m_localFile = true;
	}
	else
	{
		m_localFile = false;
	}
}

PlaylistItem::~PlaylistItem()
{

}

QVariant PlaylistItem::value( int column )
{
	switch(column)
	{
		case PlaylistItem::Artist:
			return m_artist;
		case PlaylistItem::Title:
			return m_title;
		case PlaylistItem::Album:
			return m_album;
		case PlaylistItem::Place:
			return m_place;
		case PlaylistItem::Year:
			return m_year;
		case PlaylistItem::Track:
			return m_track;
		case PlaylistItem::Length:
			return m_length;
		case PlaylistItem::Bitrate:
			return m_bitrate;
		default:
			return QVariant();
	}
}