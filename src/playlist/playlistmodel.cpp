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

#include "playlistmodel.h"

using namespace Playlist;

PlaylistModel::PlaylistModel(QObject *parent)
	: QStandardItemModel(parent)
{
	QStringList headerLabels;
	headerLabels.insert( Playlist::Artist, tr("Artist") );
	headerLabels.insert( Playlist::Title, tr("Title") );
	headerLabels.insert( Playlist::Album, tr("Album") );
	headerLabels.insert( Playlist::Track, tr("Track") );
	headerLabels.insert( Playlist::Length, tr("Length") );
	setHorizontalHeaderLabels(headerLabels);
}

PlaylistModel::~PlaylistModel()
{

}

Qt::DropActions PlaylistModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags retval = Qt::ItemIsSelectable | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	if(index.isValid())
	{
		return Qt::ItemIsDragEnabled | retval;
	}
	return retval;
}

QStringList PlaylistModel::mimeTypes() const
{
	 QStringList types;
	 types << "text/uri-list";
	 return types;
}

QMimeData * PlaylistModel::mimeData(const QModelIndexList &indexes) const
 {
	QMimeData *mimeData = new QMimeData();
	QList<QUrl> urls;

	foreach (QModelIndex index, indexes)
	{
		if (index.isValid()) 
		{
			const QUrl url = data(index, Qt::UserRole).toUrl();
			if(!urls.contains(url))
				urls.append(url);
		}
	}
	mimeData->setUrls(urls);
	return mimeData;
}

bool PlaylistModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	Q_UNUSED(column);
	if(action == Qt::IgnoreAction)
		return true;
	if(!data->hasUrls())
		return false;
	if(row == -1)
	{
		if(rowCount() == 0)
		{
			row = 0;
		}
		else
		{
			row = rowCount();
		}
	}
	insertDropData(data->urls(),row);
	return true;
}

void PlaylistModel::setPlayRow(int row)
{
	/* find old play row if it exists and reset it */
	int oldRow = getPlayRow();
	if(oldRow != -1)
	{
		for(int i = 0; i < columnCount(); i++)
		{
			item(oldRow,i)->setData(false,PlayRole);
		}
	}
	/* set new play row */
	if(row < 0 || row >= rowCount())
		return;
	for(int i = 0; i < columnCount(); i++)
	{
		item(row,i)->setData(true,PlayRole);
	}
}

/* find current play row if none set -1 returned */
int PlaylistModel::getPlayRow()
{
	for(int i = 0; i < rowCount(); i++)
	{
		if(!item(i)->data(PlayRole).toBool())
			continue;
		else
			return i;
	}
	return -1;
}

/*
void PlaylistModel::removeDirty()
{
	for(int i = rowCount(); i > 0; i--)
	{
		if(!index(i,0).data(ValidRole).toBool())
		{
			Debug << "taking row" << i << "row count:" << rowCount() << "item list text:" << itemList.size();
			takeRow(i);
			Debug << "taking out of itemList";
			delete itemList.takeAt(i -1 );
		}
	}
}
*/

void PlaylistModel::insertDropData(QList<QUrl> urls, int startRow)
{
	foreach(QUrl url,urls)
	{
		if(url.scheme() == "file")
		{
			QFileInfo urlInfo(url.toLocalFile());
			if(urlInfo.isDir())
			{
				insertDir(url,startRow++,true);
			}
			else if(urlInfo.isFile())
			{
				insertFile(url,startRow++);
			}
			else
			{
				break;
			}
		}
		else
		{
			PlaylistItem *item = new PlaylistItem(url,this);
			insertRow(startRow++,item->itemList);
			foreach(QStandardItem *child, item->itemList)
			{
				child->setData(url,Playlist::UrlRole);
			}
		}
	}
}

void PlaylistModel::insertDir(QUrl url, int row, bool recusive)
{
	if ((row < 0) || (row > rowCount()))
		row = rowCount();
	QDir rootDir(url.toLocalFile());
	Debug << rootDir.absolutePath();
	QList<QFileInfo> files = rootDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot,QDir::Name);
	QList<QFileInfo> dirs = rootDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);

	/* as long as we got files or dirs we continue */
	while(files.isEmpty() != true || dirs.isEmpty() != true)
	{
		/* insert the files, on first run these are rootDir files */
		while(!files.isEmpty())
		{
			QFileInfo file = files.takeFirst();
			insertFile(QUrl(FILE_MARCO + file.path() + "/" + file.fileName()),row++);
		}
		/* insert subdirs */
		while(!dirs.isEmpty())
		{
			/* if she is not going deep, I am sending her home. */
			if(!recusive)
			{
				return;
			}
			QDir subDir(dirs.takeFirst().filePath());
			QList<QFileInfo> subDirs = subDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
			foreach(QFileInfo subInfo, subDirs)
			{
				/* prepend subdirs into dirs, so we do not skrew up the order. */
				dirs.prepend(subInfo);
			}
			QList<QFileInfo> subFiles = subDir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot,QDir::Name);
			foreach(QFileInfo subInfo, subFiles)
			{
				/* append files under curent subdir */
				files.append(subInfo);
			}
		}
	}
}

void PlaylistModel::insertFile(QUrl url, int row)
{
	if ((row < 0) || (row > rowCount()))
		row = rowCount();
	QString file(url.toString());
	file.remove(FILE_MARCO);
#ifndef Q_WS_X11
	QByteArray ba(file.toLatin1());
	const char *tFile = ba.data();
#else
	const char *tFile = file.toUtf8();
#endif
	TagLib::FileRef f(tFile);
	if(!f.isNull() && f.tag())
    {
		// set row data
		TagLib::Tag *tag = f.tag();
		TagLib::AudioProperties *ap = f.audioProperties();
		QStandardItem *artist = new QStandardItem( QString::fromUtf8(tag->artist().toCString(true)));
		QStandardItem *title = new QStandardItem( QString::fromUtf8(tag->title().toCString(true)));
		QStandardItem *album = new QStandardItem( QString::fromUtf8(tag->album().toCString(true)));
		QStandardItem *track = new QStandardItem( QString::number(tag->track()));
		QStandardItem *itemLength = new QStandardItem();
		track->setTextAlignment(Qt::AlignCenter);
		itemLength->setTextAlignment(Qt::AlignCenter);
		
		// convert track length to string
		int length = ap->length();
		int min = length / 60;
		int sec = length % 60;
		if(sec < 10)
			itemLength->setText(QString("%1:0%2").arg(min).arg(sec));
		else
			itemLength->setText(QString("%1:%2").arg(min).arg(sec));

		// set drop daow data
		QString extendedInfo = "<?xml version=\"1.0\" encoding=\"utf-8\"?><extended>\
						   <description>%1</description>\
						   <bitrate>%2</bitrate>\
						   <year>%3</year>\
						   <location>%4</location>\
						   <imagedata dt:dt=\"binary.base64\">%5</imagedata>\
						   </extended>";

		QString description = QString::fromUtf8(tag->comment().toCString(true));
		QString bitrate = QString::number(ap->bitrate());
		QString year = QString::number(tag->year());
		
		QByteArray imageData;
		QFile coverFile(":/testcover");
		if(!coverFile.open(QIODevice::ReadOnly))
		{
			Debug << "file error";
		}
		else
		{
			imageData = coverFile.readAll().toBase64();
		}
		
        
		Debug << imageData;

		QStandardItem *dropDownInfo = new QStandardItem();
		extendedInfo = extendedInfo.arg(description,bitrate,year,file,imageData);
		dropDownInfo->setData(extendedInfo,ExtendedData);

		Debug << extendedInfo;

		artist->setChild(0,0,dropDownInfo);

		QList<QStandardItem*> rowItem;
		rowItem << artist << title << album << track << itemLength;
		foreach(QStandardItem *item, rowItem)
		{
			item->setData(Local,PlacementRole);
			item->setData(url,UrlRole);
			item->setData(true,ValidRole);
		}
		insertRow(row,rowItem);
		return;
	}
	Debug << "invalid local file (taglib data): " << tFile;
}