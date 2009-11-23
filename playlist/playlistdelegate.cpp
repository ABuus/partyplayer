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
		QPixmap playBack(":/playing");
		if(index.column() == 0)
		{
			painter->drawPixmap(rect.adjusted(0,0,(rect.width() * -1) + 10,0),playBack,QRect(0,0,10,20));
			painter->drawPixmap(rect.adjusted(10,0,0,0),playBack,QRect(20,0,0,20));
		}
		else if(index.column() == 7)
		{
			painter->drawPixmap(rect.adjusted(rect.width() -10,0,0,0),playBack,QRect(10,0,10,20));
			painter->drawPixmap(rect.adjusted(0,0,-10,0),playBack,QRect(20,0,0,20));
		}
		else
		{
			painter->drawPixmap(rect,playBack,QRect(20,0,0,20));
		}
	}
	// draw text
	if(option.state & QStyle::State_Selected || playing)
		painter->setPen(QColor(0,0,0));
	else
		painter->setPen(QColor(175,175,175));
	rect.adjust(4,4,-4,-4);
	int column = index.column() +1;
	if(column < Year || Bitrate < column )
		painter->drawText(rect, Qt::AlignVCenter, index.data().toString());
	else
		painter->drawText(rect, Qt::AlignVCenter, index.data().toString());
}
