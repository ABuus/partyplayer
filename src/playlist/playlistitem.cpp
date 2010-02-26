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

PlaylistItem::PlaylistItem(const QUrl url, QObject *parent)
	:QObject(parent)
{
	m_isValid = false;

	// create items
	m_artist = new QStandardItem("data");
	m_title = new QStandardItem("data");
	m_album = new QStandardItem("data");
	m_place = new QStandardItem("data");
	m_year = new QStandardItem("data");
	m_track = new QStandardItem("data");
	m_length = new QStandardItem("data");
	m_bitrate = new QStandardItem("data");
	itemList.append(m_artist);
	itemList.append(m_title);
	itemList.append(m_album);
	itemList.append(m_track);
	itemList.append(m_length);
	
	m_artist->setChild(0,0,m_place);
	m_artist->setChild(0,1,m_year);
	m_artist->setChild(0,2,m_bitrate);
	
	m_year->setTextAlignment(Qt::AlignCenter);
	m_track->setTextAlignment(Qt::AlignCenter);
	m_bitrate->setTextAlignment(Qt::AlignCenter);
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
	
	
	QString title = entry.elementsByTagName("title").at(0).firstChild().nodeValue();
	QString description = entry.elementsByTagName("media:description").at(0).firstChild().nodeValue();
	
	QString url = entry.elementsByTagName("id").at(0).firstChild().nodeValue();
	url.replace("http://gdata.youtube.com/feeds/api/videos/","http://www.youtube.com/watch?v=");

	// mg is a media:group see http://code.google.com/apis/youtube/2.0/reference.html#youtube_data_api_tag_media:group
	QDomElement mg = entry.elementsByTagName("media:group").at(0).toElement();
	QString duration = mg.elementsByTagName("yt:duration").at(0).toElement().attributeNode("seconds").value();
	
	QString thumbnail = mg.elementsByTagName("media:thumbnail").at(0).toElement().attributeNode("url").value();

	Debug << title << description << url << duration << thumbnail;
		
	m_artist->setText(title);
	m_title->setText(description);
	m_place->setText(url);
	
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
	setDataAll(thumbnail,ImageRole);
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