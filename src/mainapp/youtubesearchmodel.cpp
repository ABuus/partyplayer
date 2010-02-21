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

#include "youtubesearchmodel.h"

YoutubeSearchModel::YoutubeSearchModel(QObject *parent)
	: QStandardItemModel(parent),
	searchOffset(1)
{
	netGetter = new QNetworkAccessManager(this);
	connect(netGetter,SIGNAL(finished( QNetworkReply *)),this,SLOT(queryFinished(QNetworkReply *)));
	connect(this,SIGNAL(newItem(QStringList)),this,SLOT(insertSearchItem(QStringList)));
}

YoutubeSearchModel::~YoutubeSearchModel()
{

}

Qt::DropActions YoutubeSearchModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

Qt::ItemFlags YoutubeSearchModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if(index.isValid())
        return Qt::ItemIsDragEnabled | defaultFlags;
    else
        return Qt::NoItemFlags;
}

QStringList YoutubeSearchModel::mimeTypes() const
{
	QStringList types;
	types << "text/uri-list";
	return types;
}

QMimeData *YoutubeSearchModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QList<QUrl> urlList;
	foreach (QModelIndex index, indexes) 
	{
		if (index.isValid()) 
		{
			urlList << QUrl( "http://www.youtube.com/watch?v=" + data(index, Qt::UserRole+1).toString());
		}
	}
	mimeData->setUrls(urlList);
	Debug << urlList;
	return mimeData;
}

void YoutubeSearchModel::search(QString query, bool append)
{
	if(!append)
	{
		searchOffset = 1;
		clear();
	}
	query.replace(" ", "+");
	QUrl url("http://gdata.youtube.com/feeds/api/videos?q=" + query + \
		"&start-index=" + QString::number(searchOffset) + \
		"&max-results=" + QString::number(49) + \
		"&format=5&key=" + dev_id);
	netGetter->get(QNetworkRequest(url));
	searchOffset += 49;
	currentSearch = query;
}

void YoutubeSearchModel::searchMore()
{
	search(currentSearch,true);
}

void YoutubeSearchModel::queryFinished(QNetworkReply *reply)
{
	QByteArray ba = reply->readAll();
	Debug << ba.data();
	QString html(ba);

	QDomDocument doc;
	doc.setContent(html);
	QDomElement docElem = doc.documentElement();
	QDomNodeList nodeList = docElem.elementsByTagName("entry");

	for(int i = 0; i < nodeList.count(); i++)
	{
		QDomElement entry = nodeList.at(i).toElement();
		QString title = entry.elementsByTagName("title").at(0).firstChild().nodeValue();
		QString description = entry.elementsByTagName("media:description").at(0).firstChild().nodeValue();
		
		QString id = entry.elementsByTagName("id").at(0).firstChild().nodeValue();
		id.remove("http://gdata.youtube.com/feeds/api/videos/");

		// mg is a media:group see http://code.google.com/apis/youtube/2.0/reference.html#youtube_data_api_tag_media:group
		QDomElement mg = entry.elementsByTagName("media:group").at(0).toElement();
		QString duration = mg.elementsByTagName("yt:duration").at(0).toElement().attributeNode("seconds").value();

//		Debug << title << description << id << duration;
		
		QStringList item;
		item << title << description << id << duration;
		emit newItem(item);
	}
	emit searchFinished();
}

void YoutubeSearchModel::insertSearchItem(QStringList item)
{
	QStandardItem *m_item = new QStandardItem(item.first());
	m_item->setData(item.at(1),Qt::ToolTipRole);
	m_item->setData(item.at(2), Qt::UserRole +1);
	m_item->setData(item.at(3), Qt::UserRole +2);
	Debug << item.at(2);
	appendRow(m_item);
}
