/*
** This file is part of the Party Player application.
** Copyright (C) 2009  Anders Buus
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "playlistitem.h"

using namespace Playlist;

/*
 *	location can be 
 *	"file://path/to/file.mp3"
 *	"http://www.youtube.com/watch?v=YYlBQKIOb-w"
 */

PlaylistItem::PlaylistItem(const QString location,int row, QObject *parent)
	:QObject(parent)
{
	m_isValid = false;
	m_row = row;
	if(location.startsWith("file"))
	{
		localFile(location);
	}
	else if(location.startsWith("http://www.youtube.com/watch?v="))
	{
		netAccessManager = new QNetworkAccessManager(this);
		connect(netAccessManager,SIGNAL(finished( QNetworkReply *)),this,SLOT(youtubeFile(QNetworkReply *)));
		QString id = location;
		id.remove("http://www.youtube.com/watch?v=");
		QUrl data_url = ("http://gdata.youtube.com/feeds/api/videos/" + id);
		netAccessManager->get(QNetworkRequest(data_url));
	}
	else
	{
		m_localFile = false;
		m_isValid = false;
	}
}

PlaylistItem::PlaylistItem(const QUrl url,int row, QObject *parent)
	:QObject(parent)
{
	m_isValid = false;
	m_row = row;
	if(url.scheme() == "file")
	{
		localFile(url.toLocalFile());
	}
	else if(url.host().contains("youtube.com"))
	{
		netAccessManager = new QNetworkAccessManager(this);
		connect(netAccessManager,SIGNAL(finished( QNetworkReply *)),this,SLOT(youtubeFile(QNetworkReply *)));
		QString id = url.toString();
		id.remove("http://www.youtube.com/watch?v=");
		QUrl data_url = ("http://gdata.youtube.com/feeds/api/videos/" + id);
		netAccessManager->get(QNetworkRequest(data_url));
	}
	else
	{
		m_localFile = false;
		m_isValid = false;
	}
}

PlaylistItem::~PlaylistItem()
{
}

QVariant PlaylistItem::value( int column )
{
	switch(column)
	{
		case PlaylistItem::Internal:
			return m_internal;
		case PlaylistItem::Artist:
			return m_artist;
		case PlaylistItem::Title:
			return m_title;
		case PlaylistItem::Album:
			return m_album;
		case PlaylistItem::Year:
			return m_year;
		case PlaylistItem::Track:
			return m_track;
		case PlaylistItem::Length:
			return m_length;
		case PlaylistItem::Bitrate:
			return m_bitrate;
		case PlaylistItem::Place:
			return m_place;
		default:
			return QVariant();
	}
}

bool PlaylistItem::localFile(QString file)
{
	file.remove("file:///");
	QByteArray ba(file.toLatin1());
	Debug << "local file: " << ba;
	const char *tFile = ba.data();
	TagLib::FileRef f(tFile);
	if(!f.isNull() && f.tag())
    {
		TagLib::Tag *tag = f.tag();
		TagLib::AudioProperties *ap = f.audioProperties();
		m_internal = PlaylistItem::Local;
		m_artist = tag->artist().toCString(true);
		m_title = tag->title().toCString(true);
		m_album = tag->album().toCString(true);
		m_place = file;
		m_year = tag->year();
		m_track = tag->track();
		// convert track length to string
		int length = ap->length();
		int min = length / 60;
		int sec = length % 60;
		if(sec < 10)
			m_length = QString("%1:0%2").arg(min).arg(sec);
		else
			m_length = QString("%1:%2").arg(min).arg(sec);
		m_bitrate = ap->bitrate();
		m_localFile = true;
		Debug << "valid local file: " << file << m_artist << m_title;
		m_isValid = true;
		emit dataRecived(-1);
		return true;
	}
	Debug << "invalid local file: " << file;
	emit dataRecived(-1);
	return false;
}

void PlaylistItem::youtubeFile(QNetworkReply *reply)
{
	QByteArray data = reply->readAll();
	Debug << "Youtube data: ";

	int entryStart = data.indexOf("entry");
	int entryEnd = data.indexOf("entry",entryStart +1);
	
	QString entry = data.mid(entryStart,entryEnd - entryStart);

	int titleStart = entry.indexOf("<title type='text'>") + 19;
	int titleEnd = entry.indexOf("</title>");

	int decStart = entry.indexOf("<media:description type='plain'>") +32;
	int decEnd = entry.indexOf("</media:description>");
	
	int idStart = entry.indexOf("<id>") +4;
	int idEnd = entry.indexOf("</id>", idStart);

	int durationStart = entry.indexOf("<yt:duration seconds='") +22;
	int durationEnd = entry.indexOf("'/>", durationStart);

	QString title = entry.mid(titleStart, (titleEnd - titleStart));
	QString dec = entry.mid(decStart, (decEnd - decStart));
	QString id = entry.mid(idStart, (idEnd - idStart));
	QString duration = entry.mid(durationStart, (durationEnd - durationStart));

	id.remove("http://gdata.youtube.com/feeds/api/videos/");

	Debug << title << id << duration;

	QString url("http://www.youtube.com/watch?v=");
	url.append(id);

	title.replace("&amp;","&");
	dec.replace("&amp;","&");

	m_internal = PlaylistItem::Youtube;
	m_artist = title;
	m_title = dec;
	m_place = url;
	m_year = 0;
	m_bitrate = 0;
	m_track = 0;
	// convert track length to string
	int length = duration.toInt();
	int min = length / 60;
	int sec = length % 60;
	if(sec < 10)
		m_length = QString("%1:0%2").arg(min).arg(sec);
	else
		m_length = QString("%1:%2").arg(min).arg(sec);
	m_localFile = false;
	emit dataRecived(m_row);
	m_isValid = true;
}