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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QTableView>
#include <QDragMoveEvent>
#include <QUrl>
#include <QPoint>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include <QHeaderView>
#include <QDir>
#include "debug.h"
#include "playlist_global.h"
#include "playlist_export.h"
#include "playlistmodel.h"
#include "playlistdelegate.h"
#include "playlistdialog.h"
#include "playlistcontextmenu.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistView : public QTableView
{
	Q_OBJECT
public:
	PlaylistView(QWidget *parent = 0);
	~PlaylistView();
	void createConnections();
	void setModel(PlaylistModel *model);
	QUrl next(bool set = true);
	QUrl previous(bool set = true);
public slots:
	void clear();
	void save();
signals:
	void playRequest(const QUrl url);
private:
	PlaylistModel *m_model;
	int m_hoverRow;
	PlaylistContextMenu *contexMenu;
private slots:
	void onDoubleClicked(const QModelIndex &index);
	void removeSelected();
protected:
	void startDrag(Qt::DropActions supportenDropActions);
	void dropEvent(QDropEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void leaveEvent(QEvent *);
	void contextMenuEvent(QContextMenuEvent *event);
};

}; // namespace Playlist

#endif // PLAYLIST_H
