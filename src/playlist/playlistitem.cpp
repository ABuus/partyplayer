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
/*
#include "playlistitem.h"

using namespace Playlist;

PlaylistItem::PlaylistItem(const QUrl url, QObject *parent)
	:QObject(parent)
{
	netAccessManager = new QNetworkAccessManager(this);
	m_isValid = false;

	// create items
	m_artist = new QStandardItem();
	m_title = new QStandardItem();
	m_album = new QStandardItem();
	m_track = new QStandardItem();
	m_length = new QStandardItem();
	m_dropDownInfo = new QStandardItem();
	itemList.append(m_artist);
	itemList.append(m_title);
	itemList.append(m_album);
	itemList.append(m_track);
	itemList.append(m_length);

	m_artist->setChild(0,0,m_dropDownInfo);
	
	m_track->setTextAlignment(Qt::AlignCenter);
	m_length->setTextAlignment(Qt::AlignCenter);
	foreach(QStandardItem *item, itemList)
	{
		setFlags(item);
	}
	
	if(url.host().contains("youtube.com"))
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
		Debug << "invalid playlist item file";
		m_localFile = false;
		m_isValid = false;
	}
}

PlaylistItem::~PlaylistItem()
{
	foreach(QStandardItem *item, itemList)
	{
		delete item;
	}
}

void PlaylistItem::youtubeFile(QNetworkReply *reply)
{
	
	QByteArray ba = reply->readAll();
	QString html(ba);

	QDomDocument doc;
	doc.setContent(html);
	QDomElement entry = doc.documentElement();
	QDomElement mediaGroup = entry.firstChildElement("media:group");
	
	QString title = entry.firstChildElement("title").text();
	m_decription = mediaGroup.firstChildElement("media:description").text();
	m_location = entry.firstChildElement("id").text();
	QString duration = mediaGroup.firstChildElement("yt:duration").attribute("seconds");
	m_thumbnailUrl = mediaGroup.firstChildElement("media:thumbnail").attribute("url");

	m_location.replace("http://gdata.youtube.com/feeds/api/videos/","http://www.youtube.com/watch?v=");

	m_decription = m_decription.simplified();
	m_artist->setText(title);
//	m_title->setText(m_decription);
	
	// convert track length to string
	int length = duration.toInt();
	int min = length / 60;
	int sec = length % 60;
	if(sec < 10)
		m_length->setText(QString("%1:0%2").arg(min).arg(sec));
	else
		m_length->setText(QString("%1:%2").arg(min).arg(sec));
	m_localFile = false;
	m_isValid = true;
	setDataAll(Youtube,PlacementRole);
	setDataAll(true,ValidRole);
//	setDataAll(thumbnail,ImageRole);

	netAccessManager->disconnect(this);
	connect(netAccessManager,SIGNAL(finished( QNetworkReply *)),this,SLOT(extendedInfoReply(QNetworkReply *)));
	netAccessManager->get( QNetworkRequest(QUrl(m_thumbnailUrl)));
	
}

void PlaylistItem::setFlags(QStandardItem *item)
{
	item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled );
}

void PlaylistItem::setDataAll(QVariant value, int role)
{
	foreach(QStandardItem *item, itemList)
	{
		item->setData(value,role);
	}
}

void PlaylistItem::extendedInfoReply(QNetworkReply *reply)
{
*/
	/* convert reply data into pixmap */
/*
	QPixmap pixmap;
	QByteArray imageData = reply->readAll().toBase64();
	pixmap.loadFromData( imageData.fromBase64(imageData) );	
*/
	/* set data */ 
/*
	m_dropDownInfo->setData(Playlist::Youtube,Playlist::PlacementRole);
	m_dropDownInfo->setData(pixmap, Playlist::ExtendedDataImage);
	m_dropDownInfo->setData(m_decription, Playlist::ExtendedDataDescription);
	m_dropDownInfo->setData(m_location, Playlist::UrlRole);

}
*/