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

#include "playlistdelegate.h"

using namespace Playlist;

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt(option);
	QRect rect = opt.rect;
	bool playing = index.data(PlayRole).toBool();

	// paint background if item is playing
	if(playing)
	{
		QLinearGradient gradient(0,0,0,1);
		gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
		QGradientStops stops;
		/* top = 0.0 bottom = 1.0 */
		stops.append(QPair<qreal,QColor>(0.0, Qt::white));
		stops.append(QPair<qreal,QColor>(0.1, QColor(255,0,255,200)));
		stops.append(QPair<qreal,QColor>(0.8, QColor(255,0,255,200)));
		stops.append(QPair<qreal,QColor>(1.0, Qt::white));
		
		gradient.setStops(stops);
		QBrush brush(gradient);
		
		// painter->fillRect(painter->viewport(), brush);
		painter->fillRect(rect,brush);
	}
	if(index.data(Qt::UserRole +5).toBool())
	{
		QLinearGradient gradient(0,0,0,1);
		gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
		QGradientStops stops;
		/* top = 0.0 bottom = 1.0 */
		stops.append(QPair<qreal,QColor>(0.0, Qt::white));
		stops.append(QPair<qreal,QColor>(0.1, QColor(255,0,255,100)));
		stops.append(QPair<qreal,QColor>(0.8, QColor(255,0,255,100)));
		stops.append(QPair<qreal,QColor>(1.0, Qt::white));
		
		gradient.setStops(stops);
		QBrush brush(gradient);
		painter->fillRect(rect,brush);
	}
	QStyledItemDelegate::paint(painter,option,index);
}

