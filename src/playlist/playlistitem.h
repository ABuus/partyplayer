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

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

// Qt
#include <QStandardItem>
#include <QVariant>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
// taglib
#include <fileref.h>
#include <tag.h>
#include <audioproperties.h>
#include "playlist_export.h"
#include "debug.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistItem : public QObject
{
	Q_OBJECT
public:
	PlaylistItem(const QString location,int row = -1, QObject *parent = 0);
	PlaylistItem(const QUrl url,int row = -1, QObject *parent = 0);
	~PlaylistItem();
	QVariant value( int column );
	QString artist() { return m_artist; };
	QString title() { return m_title; };
	QString album() { return m_album; };
	QString place() { return m_place; };
	int year() { return m_year; };
	int track() { return m_track; };
	QString length() { return m_length; };
	int bitrate() { return m_bitrate; };
	bool isLocal() { return m_localFile; };
	bool isValid() { return m_isValid; };
	enum ColumnData {
		Internal = 0,
		Artist,
		Title,
		Album,
		Year,
		Track,
		Length,
		Bitrate,
		Place
	};
	enum InternalData {
		Local = 0,
		Youtube
	};
private:
	int m_internal;
	QString m_artist;
	QString m_title;
	QString m_album;
	QString m_place;
	int m_year;
	int m_track;
	QString m_length;
	int m_bitrate;
	bool m_localFile;
	bool m_isValid;
	int m_row;
	QNetworkAccessManager *netAccessManager;
	bool localFile(QString file);
private slots:
	void youtubeFile(QNetworkReply *reply);
signals:
	void dataRecived(int row);
	
};

}; // namespace Playlist

#endif // PLAYLISTITEM_H
