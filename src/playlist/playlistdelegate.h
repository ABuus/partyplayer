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

#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QBrush>
#include <QRect>
#include <QFont>
#include "../debug.h"
#include "playlist_global.h"
#include "playlist_export.h"

#define EXTENDED_INFO_HEIGHT 80
#define EXTENDED_INFO_INDENTATION 22

namespace Playlist {

class PLAYLIST_EXPORT PlaylistDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	PlaylistDelegate(QObject *parent);
	~PlaylistDelegate();
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QRectF extendedHandleRect() { return m_handleRect; };
	bool extendedUrl(int myWidth, QPoint p );
private:
	const QPixmap bgTexture;
	const QPixmap handleLess;
	const QPixmap handleMore;
	QRectF m_handleRect;
	QRectF m_locationRect; // see paint for real value
	QFont monoFont;
	const QFont timesFont;
};

}; // namespace Playlist

#endif // PLAYLISTDELEGATE_H
