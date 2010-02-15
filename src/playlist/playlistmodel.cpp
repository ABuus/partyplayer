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
	headerLabels.insert( Playlist::Year, tr("Year") );
	headerLabels.insert( Playlist::Track, tr("Track") );
	headerLabels.insert( Playlist::Length, tr("Length") );
	headerLabels.insert( Playlist::Bitrate, tr("Bitrate") );
	headerLabels.insert( Playlist::Place, tr("Directory") );
	headerLabels.insert( Playlist::Internal, tr("Internal") );
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
	return insertUrls(data->urls(),row);
}

bool PlaylistModel::insertUrls(const QList<QUrl> urls, int startRow)
{
	QList<QUrl> validLocations;
	foreach(QUrl url, urls)
	{
		if(isDirectory(url))
		{
			insertAllFilesUnderDirectory(url);
		}
		else
		{
			validLocations.append(url);
		}
	}

	/* insert all valid data */
	bool retval = true;
	foreach(QUrl validLocation, validLocations)
	{
		if(startRow == -1 && rowCount() == 0)
		{
			startRow = 0;
			if(!insertUrl(validLocation,startRow))
				retval = false;
		}
		else if(startRow == -1)
		{
			startRow = rowCount();
			if(!insertUrl(validLocation,startRow))
				retval = false;
		}
		else
		{
			if(!insertUrl(validLocation,++startRow))
				retval = false;
		}
	}
	return retval;
}

/* 
 * inserts url at row if row is -1 the item is appended to the model 
 */

bool PlaylistModel::insertUrl(const QUrl &url, int row)
{
	if(!url.isValid())
		return false;
	if(row == -1)
		row = rowCount();
	PlaylistItem *item = new PlaylistItem(this);
	connect(item,SIGNAL(dataRecived()),this,SLOT(updateItemData()));
	QList<QStandardItem*> rowItem;
	for(int i = 0; i < columnCount(); i++)
	{
		QStandardItem *stdItem = new QStandardItem(item->value(i).toString());
		stdItem->setData(url,Playlist::UrlRole);
		stdItem->setData(false,Playlist::PlayRole);
		if(i == Year || i == Track || i == Length || i == Bitrate)
		{
			stdItem->setTextAlignment(Qt::AlignCenter);
		}
		rowItem << stdItem;
	}
	item->setInternalPointer(rowItem.last());
	insertRow(row,rowItem);
	item->setUrl(url);
	Debug << "inserted" << url.toString() << "at" << row;
	return true;
}

/*
 * this slot is connected to Playlist::PlaylistItem::dataReciced() signal
 * this updates data at the QModelIndex PlaylistItem::internelPointer()
 * WARNING: The sender object should be deleted in this slot
 */

void PlaylistModel::updateItemData()
{
	PlaylistItem *playlistItem = qobject_cast<PlaylistItem*>( sender() );
	QStandardItem *pointer = playlistItem->internalPointer();
	int row = indexFromItem(pointer).row();
	/* if the item could not gather valid info delete it and its row */
	if(!playlistItem->isValid())
	{
		Debug << "invalid item";
		removeRow(row);
		delete playlistItem;
		return;
	}
	/* if item´s internal pointer is 0 delete it */
	if(pointer == 0)
	{
		playlistItem->deleteLater();
		return;
	}
	/* valid data insert */
	for(int i = 0; i < columnCount(); i++)
	{
		setData(index(row,i),playlistItem->value(i),Qt::DisplayRole);
	}
	/* delete the item */
	playlistItem->deleteLater();
}

void PlaylistModel::setPlayRow(int row)
{
	/* find old play row if it exists and reset it */
	for(int i = 0; i < rowCount(); i++)
	{
		if(!item(i)->data(PlayRole).toBool())
			continue;
		for(int j = 0; j < columnCount(); j++)
		{
			item(i,j)->setData(false,PlayRole);
		}
	}
	/* set new play row */
	for(int i = 0; i < columnCount(); i++)
	{
		item(row,i)->setData(true,PlayRole);
	}
}

bool PlaylistModel::isDirectory(const QUrl url)
{
/*
	if(url.isValid())
		return false;
*/
	bool retval = false;
	if(url.scheme() != "file")
		retval = false;
	else
	{
		QFileInfo info(url.toLocalFile());
		retval = info.isDir();
	}
	Debug << "Found dir:" << retval << "at" << url.toString();
	return retval;
}

/*
 * inserts all files under url recursively.
 * url has to be local dir. eg. file:///C:/Users/User/Music
 */

void PlaylistModel::insertAllFilesUnderDirectory(const QUrl url)
{
	QFileInfo topInfo(url.toLocalFile());
	Q_ASSERT(topInfo.exists());
	QFlags<QDir::Filter> dirFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	QFlags<QDir::Filter> fileFilter(QDir::Files | QDir::NoDotAndDotDot);
	QFlags<QDir::SortFlag> dirSortFlags(QDir::Name);

	QDir topDir( topInfo.path() + "/" + topInfo.fileName(), "");
	topDir.setSorting(dirSortFlags);

	/* insert all files under topdir */
	topDir.setFilter(fileFilter);
	QList<QFileInfo> topFiles = topDir.entryInfoList();
	foreach(QFileInfo file, topFiles)
	{
		QUrl url = FILE_MARCO + file.path() + "/" + file.fileName();
		Debug << "found file:" << url.toString();
		insertUrl(url);
	}
	
	/* insert all dirs and there files under topdir */
	topDir.setFilter(dirFilter);
	QList<QFileInfo> subDirs;
	const QStringList strSubDirs = topDir.entryList();
	foreach(QString strSubDir, strSubDirs)
	{
		QFileInfo info(topInfo.path() + "/" + topInfo.fileName() + "/" + strSubDir);
		subDirs.append(info);
	}
	while(!subDirs.isEmpty())
	{
		QFileInfo info(subDirs.takeFirst());
		QDir sub( info.path() + "/" + info.fileName());
		/* get all files under sub */
		sub.setFilter(fileFilter);
		QStringList entList = sub.entryList();
		foreach(QString file, entList)
		{
			QUrl url = FILE_MARCO + info.path() + "/" + info.fileName() + "/" + file;
			insertUrl(url);
		}
		/* get all sub dirs add them to subDirs */
		sub.setFilter(dirFilter);
		sub.setSorting(dirSortFlags);
		entList = sub.entryList();
		Debug << "subdies to be inserted" << entList.count() << info.path() + "/" + info.fileName();
		if(!entList.isEmpty())
		{
			foreach(QString strSubDir, entList)
			{
				QFileInfo subInfo( info.path() + "/" + info.fileName() + "/" + strSubDir);
				subDirs.append(subInfo);
			}
		}
	}
}