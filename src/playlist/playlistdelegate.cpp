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
	handleLess(":/handle_less"),
	handleMore(":/handle_more"),
	m_handleRect(5.0,5.0,10.0,10.0)
{}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt(option);
	QRectF rectF = opt.rect;
	bool playing = index.data(PlayRole).toBool();   // the item is in play state
	bool expanded = opt.state & QStyle::State_Open; // the item is expanded
	
	
	painter->save();
	painter->setPen(Qt::NoPen);
	QPainterPath backgroundPath;
	QPixmap bgTexture(":/texture");
	
	backgroundPath.addRect(rectF);

	/* setup background bruches */
	if(!index.parent().isValid())
	{
		painter->fillRect(rectF,Qt::black);
		painter->setBrush(bgTexture.scaledToHeight(rectF.height()));
	}
	else
	{
		painter->setBrush(QColor(0,0,0,55));
	}
	
	if(playing)
	{
		if(!index.parent().isValid())
			painter->setOpacity(0.9);
	}
	/* paint selected */
	else if(opt.state & QStyle::State_Selected)
	{
		if(!index.parent().isValid())
			painter->setOpacity(0.8);
	}

	/* Paint background */
	
	painter->drawPath(backgroundPath);
	painter->restore();

	/* draw drop down handle, only parents have handles */
	if(!index.parent().isValid() && index.column() == 0)
	{
		painter->save();
		QRectF handleOffsetRect = m_handleRect.adjusted(0,rectF.y(),0,rectF.y());
		painter->drawEllipse(handleOffsetRect);
		painter->drawLine(handleOffsetRect.left()+3,handleOffsetRect.center().y(), 
				handleOffsetRect.right()-3, handleOffsetRect.center().y());
		if(!expanded)
		{
			painter->drawLine(handleOffsetRect.center().x(),handleOffsetRect.top()+3, 
				handleOffsetRect.height(), handleOffsetRect.bottom()-3);
		}
		painter->restore();
	}

	/* drop down */
	if(index.parent().isValid())
	{
		// draw forground
		QDomDocument doc;
		doc.setContent(index.data(Playlist::ExtendedData).toString());
		QDomElement entry = doc.firstChildElement("extended");
		QString description = entry.firstChildElement("description").text();
		QString bitrate = entry.firstChildElement("bitrate").text();
		QString year = entry.firstChildElement("year").text();
		QString location = entry.firstChildElement("location").text();
		QPixmap pixmap;
		QString imageStr = entry.firstChildElement("imagedata").text();
		QByteArray imagedata = imageStr.toAscii();
		pixmap.loadFromData( imagedata.fromBase64(imagedata));

		description.prepend(tr("Description: "));
		bitrate.prepend(tr("Bitrate: "));
		year.prepend(tr("Year: "));

		double WHoffset = (double)pixmap.width() / pixmap.height();
		QRectF imageRectF(rectF.topLeft().x(),rectF.topLeft().y(),(EXTENDED_INFO_HEIGHT*WHoffset) - 10,EXTENDED_INFO_HEIGHT -10);
		imageRectF.adjust(70,5,70,5);

		QRectF descriptionRectF(rectF.adjusted(200,0,0,0));
		descriptionRectF.setWidth(250);

		QRectF locationRectF(rectF.adjusted(500,rectF.height()-(EXTENDED_INFO_HEIGHT/4),0,0));
		locationRectF.setWidth(280);

		QRectF bitrateRectF(rectF.adjusted(500,rectF.height()- 2*(EXTENDED_INFO_HEIGHT/4),0,0));
		bitrateRectF.setWidth(280);
		
		QRectF yearRectF(rectF.adjusted(500,rectF.height()- 3*(EXTENDED_INFO_HEIGHT/4),0,0));
		yearRectF.setWidth(280);
		
		painter->drawPixmap(imageRectF,pixmap,pixmap.rect());
		painter->drawText(descriptionRectF,Qt::AlignLeft|Qt::TextWordWrap,description);
		painter->drawText(bitrateRectF,Qt::AlignLeft|Qt::TextWordWrap,bitrate);
		painter->drawText(yearRectF,Qt::AlignLeft|Qt::TextWordWrap,year);
		
		painter->save();
		QFont monoFont("monospace", 8);
		monoFont.setUnderline(true);
		painter->setPen(Qt::blue);
		painter->setFont(monoFont);
		painter->drawText(locationRectF,Qt::AlignLeft,location);
		painter->restore();
	}
	else
	{
		/* handle offset */
		if(opt.viewItemPosition == QStyleOptionViewItemV4::Beginning)
			rectF.adjust(22,0,0,0);
		/* draw text parent items */
		rectF.adjust(2,2,-2,-2);
		
		painter->save();
		QFont font("Times", 10, QFont::Bold);
		painter->setPen(QColor(0,0,0,200));
		painter->setFont(font);
		QFlags<Qt::Alignment> flags( index.data(Qt::TextAlignmentRole).toInt() );
		painter->drawText(rectF.left(),rectF.top(),rectF.width(),rectF.height(),flags,index.data().toString());
		painter->restore();
	}
}


QSize PlaylistDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(!index.parent().isValid())
	{
		const QSize size(option.rect.width(), 20);
		return size;
	}
	else
	{
		const QSize size(option.rect.width(), EXTENDED_INFO_HEIGHT);
		return size;
	}
}

