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
/*
// Qt
#include <QStandardItem>
#include <QVariant>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomNode>
#include "playlist_export.h"
#include "playlist_global.h"
#include "../debug.h"


namespace Playlist {

class PLAYLIST_EXPORT PlaylistItem : public QObject
{
	Q_OBJECT
public:
	PlaylistItem(const QUrl location, QObject *parent);
	~PlaylistItem();
	QList<QStandardItem*> itemList;
	bool IsValid() { return m_isValid; };
private:
	QStandardItem *m_artist;
	QStandardItem *m_title;
	QStandardItem *m_album;
	QStandardItem *m_track;
	QStandardItem *m_length;
	QStandardItem *m_dropDownInfo;
	QString m_decription;
	QString m_thumbnailUrl;
	QString m_location;
	bool m_localFile;
	bool m_isValid;
	QNetworkAccessManager *netAccessManager;
	QUrl m_url;
	void setFlags(QStandardItem *item);
	void setDataAll(QVariant value,int role);
private slots:
	void youtubeFile(QNetworkReply *reply);
	void extendedInfoReply(QNetworkReply *reply);
signals:
};

}; // namespace Playlist
*/
#endif // PLAYLISTITEM_H
