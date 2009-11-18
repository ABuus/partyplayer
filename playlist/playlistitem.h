#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

// Qt
#include <QStandardItem>
#include <QVariant>
#include <QUrl>
// taglib
#include <fileref.h>
#include <tag.h>
#include <audioproperties.h>

namespace Playlist {

class PlaylistItem
{
public:
	PlaylistItem(const QUrl url, const QString ytText = 0);
	~PlaylistItem();
	QVariant value( int column );
	QString artist() { return m_artist; };
	QString title() { return m_title; };
	QString album() { return m_album; };
	QString place() { return m_place; };
	int year() { return m_year; };
	int track() { return m_track; };
	int length() { return m_length; };
	int bitrate() { return m_bitrate; };
	bool isLocal() { return m_localFile; };
	bool isValid() { return m_isValid; };
	enum ColumnData {
		Artist = 0,
		Title,
		Album,
		Year,
		Track,
		Length,
		Bitrate,
		Place
	};
private:
	QString m_artist;
	QString m_title;
	QString m_album;
	QString m_place;
	int m_year;
	int m_track;
	int m_length;
	int m_bitrate;
	bool m_localFile;
	bool m_isValid;
	bool localFile(const QString &file);
	void youtubeFile(const QUrl &url, const QString ytText);
};

}; // namespace Playlist

#endif // PLAYLISTITEM_H
