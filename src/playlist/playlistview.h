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
#include "playlistitem.h"
#include "playlistdelegate.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistView : public QTableView
{
	Q_OBJECT
public:
	PlaylistView(QWidget *parent = 0);
	~PlaylistView();
	QVariant next();
	QVariant previous();
	bool addFile(QString &file, int row = -1); // if row = -1 item id added at bottom
public slots:
	void clear();
	void handleItemData(int row);
signals:
	void playRequest(const QVariant value);
private:
	bool addM3U(const QString file, int row); // if row = -1 item id added at bottom
	bool addYoutube(const QUrl url, int row = -1); // if row = -1 item id added at bottom
	bool addYoutube(const QString title,const QString description, 
		const QString vidId, const QString duration, int row);	// if row = -1 item id added at bottom
	bool addDir(QString path, int row); // if row = -1 item id added at bottom
	void setPlayRow(int row, bool playing = false);
	PlaylistModel *m_model;
	QPoint startDragPos;
	int m_dragRow;
	int m_playRow;
	bool m_dragPlaying;
protected:
	void dragMoveEvent(QDragMoveEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void dropEvent(QDropEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
};

}; // namespace Playlist

#endif // PLAYLIST_H
