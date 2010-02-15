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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QStandardItemModel>
#include <QMimeData>
#include <QModelIndex>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include "debug.h"
#include "playlist_global.h"
#include "playlist_export.h"
#include "playlistitem.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistModel : public QStandardItemModel
{
	Q_OBJECT

public:
	PlaylistModel(QObject *parent);
	~PlaylistModel();
	Qt::DropActions supportedDropActions() const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QStringList mimeTypes() const;
	QMimeData * mimeData(const QModelIndexList &indexes) const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
	bool insertUrls(const QList<QUrl> urls, int startRow);
	bool insertUrl(const QUrl &url, int row = -1);
	void setPlayRow(int row);
private:
	bool isDirectory(const QUrl url);
	void insertAllFilesUnderDirectory(const QUrl url);
private slots:
	void updateItemData();
};

}; // namespace Playlist

#endif // PLAYLISTMODEL_H
