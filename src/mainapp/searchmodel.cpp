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

#include "searchmodel.h"

SearchModel::SearchModel(QObject *parent)
	: QStandardItemModel(parent)
{

}

SearchModel::~SearchModel()
{

}

Qt::DropActions SearchModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

Qt::ItemFlags SearchModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if(index.isValid())
        return Qt::ItemIsDragEnabled | defaultFlags;
    else
        return Qt::NoItemFlags;
}

QStringList SearchModel::mimeTypes() const
{
	QStringList types;
	types << "text/uri-list";
	return types;
}

QMimeData *SearchModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();

	QByteArray ba;

	foreach (QModelIndex index, indexes) 
	{
		if (index.isValid()) 
		{
			// this does not support mulit drag.
			// order { title ,description, id, duration }
			ba.append( data(index, Qt::DisplayRole).toString());
			ba.append("*\:\*");
			ba.append( data(index, Qt::ToolTipRole).toString());
			ba.append("*\:\*");
			ba.append( data(index, Qt::UserRole+1).toString());
			ba.append("*\:\*");
			ba.append( data(index, Qt::UserRole +2).toString());
		}
	}
	mimeData->setData("application/yt-partyplayer",ba);
	Debug << ba;
	return mimeData;
}

