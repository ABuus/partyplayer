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

PlaylistDelegate::PlaylistDelegate(QObject *parent)
	: QStyledItemDelegate(parent),
	bgTexture(":/texture"),
	handleLess(":/less"),
	handleMore(":/more"),
	m_handleRect(5.0,5.0,10.0,10.0),
	monoFont("monospace", 8),	
	timesFont("Times",10,QFont::Bold)
{
	monoFont.setUnderline(true);
}

PlaylistDelegate::~PlaylistDelegate()
{
}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt(option);
	QRectF rectF = opt.rect;
	bool playing = index.data(PlayRole).toBool();   // the item is in play state
	bool expanded = opt.state & QStyle::State_Open; // the item is expanded
	
	/* ---- background painting -------- */
	
	painter->save();
	painter->setPen(Qt::NoPen);

	/* setup background bruches */
	if(!index.parent().isValid()) // main info
	{
		painter->fillRect(rectF,Qt::black);
		painter->setBrush(bgTexture.scaledToHeight(rectF.height()));
		
		if(playing)
		{
			painter->setOpacity(0.65);
		}
		/* paint selected */
		else if(opt.state & QStyle::State_Selected)
		{
			painter->setOpacity(0.8);
		}
	}
	else // extended info(drop down)
	{
		painter->setBrush(QColor(0,0,0,55));
	}
	
	/* Paint background */
	
	painter->drawRect(rectF);
	painter->restore();

	/* ---- dropdown handle painting -------- */

	/* draw drop down handle, only parents have handles */
	if(!index.parent().isValid() && index.column() == 0)
	{
		QRectF handleOffsetRect = m_handleRect.adjusted(0,rectF.y(),0,rectF.y());
		if(expanded)
			painter->drawPixmap(handleOffsetRect,handleLess,handleLess.rect());
		else
			painter->drawPixmap(handleOffsetRect,handleMore,handleMore.rect());
	}

	/* ---- forground painting -------- */

	/* drop down */
	if(index.parent().isValid())
	{
		// draw forground
		const qreal yAlign = rectF.topLeft().y()+5;
		const qreal rightMargin = rectF.right()-EXTENDED_INFO_INDENTATION;
		QPixmap pixmap = qvariant_cast<QPixmap>( index.data(ExtendedDataImage));

		double WHoffset = (double)pixmap.width() / pixmap.height();
		QRectF imageRectF(EXTENDED_INFO_INDENTATION,yAlign,(EXTENDED_INFO_HEIGHT*WHoffset) - 10,EXTENDED_INFO_HEIGHT -10);
		/* the x width offset(300-280=20px) is so we line up with the values of year,bitrate & samplerate */
		QRectF locationRectF(rightMargin-280,yAlign+EXTENDED_INFO_HEIGHT-20,300,15);
		QRectF yearRectF(rightMargin-120,yAlign,120,15);
		
		/* draw description */
		painter->drawPixmap(imageRectF,pixmap,pixmap.rect());
		painter->drawText(QRectF(EXTENDED_INFO_INDENTATION+imageRectF.width()+10,yAlign,300,EXTENDED_INFO_HEIGHT -5),Qt::AlignLeft|Qt::TextWordWrap,index.data(ExtendedDataDescription).toString());
		/* draw year,bitrate & samplerate */
		painter->drawText(yearRectF,tr("Year:"));
		painter->drawText(yearRectF.adjusted(40,0,20,0),Qt::AlignRight,index.data(ExtendedDataYear).toString());
		painter->drawText(yearRectF.adjusted(0,15,0,15),tr("Bitrate:"));
		painter->drawText(yearRectF.adjusted(40,15,20,15),Qt::AlignRight,index.data(ExtendedDataBitrate).toString());
		painter->drawText(yearRectF.adjusted(0,30,0,30),tr("Sample rate:"));
		painter->drawText(yearRectF.adjusted(40,30,20,30),Qt::AlignRight,index.data(ExtendedDataBitrate).toString());
		/* draw url */
		painter->save();
		painter->setPen(Qt::blue);
		painter->setFont(monoFont);
		QFontMetrics foo(monoFont);
		QString location = index.data(UrlRole).toString();
		if(location.startsWith(FILE_MARCO)) // this should be moved out of paint
			location.remove(FILE_MARCO);
		location = foo.elidedText(location,Qt::ElideMiddle,locationRectF.width(),Qt::AlignRight);
		painter->drawText(locationRectF,Qt::AlignRight,location);
		painter->restore();
	}
	else
	{
		/* handle offset in column 0 */
		if(opt.viewItemPosition == QStyleOptionViewItemV4::Beginning)
			rectF.adjust(22,0,0,0);
		/* draw text parent items */
		rectF.adjust(2,3,-2,-2);
		
		painter->save();
		painter->setPen(QColor(0,0,0,200));
		painter->setFont(timesFont);
		QFlags<Qt::Alignment> flags( index.data(Qt::TextAlignmentRole).toInt() );
		painter->drawText(rectF.left(),rectF.top(),rectF.width(),rectF.height(),flags,index.data().toString());
		painter->restore();
	}
}


QSize PlaylistDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(!index.parent().isValid())
	{
		QFontMetrics fontMetrics(QFont("Times", 10, QFont::Bold));
		QSize size(fontMetrics.width(index.data().toString()) + 10, 20);
		if(index.column() == 0)
		{
			size.setWidth(size.width() +22);
		}
		return size;
	}
	else
	{
		const QSize size(option.rect.width(), EXTENDED_INFO_HEIGHT);
		return size;
	}
}

bool PlaylistDelegate::extendedUrl(int myWidth, QPoint point)
{
	QRectF urlRect(myWidth-300,EXTENDED_INFO_HEIGHT-20,300,15);
	return urlRect.contains(point);
}