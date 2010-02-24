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
	m_artist = new QStandardItem();
	m_title = new QStandardItem();
	m_album = new QStandardItem();
	m_place = new QStandardItem();
	m_year = new QStandardItem();
	m_track = new QStandardItem();
	m_length = new QStandardItem();
	m_bitrate = new QStandardItem();
	itemList.append(m_artist);
	itemList.append(m_title);
	itemList.append(m_album);
	itemList.append(m_year);
	itemList.append(m_track);
	itemList.append(m_length);
	itemList.append(m_bitrate);
	itemList.append(m_place);
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

	m_artist->setText(title);
	m_title->setText(dec);
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