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

#ifndef YOUTUBEITEM_H
#define YOUTUBEITEM_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDomDocument>
#include <QDomElement>
#include "playlistitem_global.h"
#include "playlistitem.h"
#include "../debug.h"

// google developer id of Anders Buus
#define dev_id "AI39si61cUdAWBI2eFoGoRKodF3zuFDyUTTFHyK_X85TDEpjIv6EgeJokdy6UiuG4bq1zri9l4gExE6oEb_xwyAXuxuUgdwdRg"

namespace PlaylistItem {

class PLAYLISTITEM_EXPORT YoutubeItem : public PlaylistItem
{
	Q_OBJECT

public:
	YoutubeItem(const QString &url,QObject *parent = 0);
	YoutubeItem(const QUrl &url,QObject *parent = 0);
	~YoutubeItem();
	QString artist() const { return m_artist; };
	QString comment() const { return m_comments; };
	uint length() const { return m_length; };
	QPixmap image() const { return m_image; };
	bool isAsync() const { return true; };
	void fetchData();
private:
	QString m_artist;
	QString m_comments;
	uint m_length;
	QString m_thumbnailUrl;
	QPixmap m_image;
	void init();
	enum ReplyType { NoType, XmlType, ThumbnailType };
	int m_replyType;
	QNetworkAccessManager *manager;
	void phraseXmlData(const QByteArray &xmlData);
	void saveToImage(const QByteArray &data);
	int type() const { return PlaylistItem::Youtube; };
private slots:
	void requestFinished(QNetworkReply *reply);	
};

}; // namespace PlaylistItem

#endif // YOUTUBEITEM_H
