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

PlaylistItem::PlaylistItem(const QUrl url, const QString ytText)
{
	m_isValid = false;
	QList<QStandardItem *> row;
	QString file;
	if(url.scheme() == "file")
	{
		if(localFile(url.toLocalFile()))
		{
			m_isValid = true;
		}
		else
		{
			m_isValid = false;
		}
	}
	else if(url.scheme() == "http" && url.host() == "www.youtube.com")
	{
		youtubeFile(url,ytText);
		m_isValid = true;
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

bool PlaylistItem::localFile(const QString &file)
{
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
		return true;
	}
	return false;
}

void PlaylistItem::youtubeFile(const QUrl &url, const QString ytText)
{
	QStringList texts = ytText.split(":");
	m_length = texts.last().toInt();
	QStringList strs = texts.first().split("-");
	m_artist = strs.first();
	m_title = strs.last();
	m_place = url.toString();
	m_year = 0;
	m_bitrate = 0;
	m_track = 0;
	m_localFile = false;
}