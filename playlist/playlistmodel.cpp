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
			urls << data(index, Qt::UserRole).toUrl();
		}
	}
	mimeData->setUrls(urls);
	return mimeData;
}

void PlaylistModel::setRowData(int row, const QVariant &value, int role)
{
	for(int i = 0; i < columnCount(); i++)
	{
	//	qDebug() << "setting data at" << i << "to" << value;
		setData( createIndex(row,i), value, role);
	}
}