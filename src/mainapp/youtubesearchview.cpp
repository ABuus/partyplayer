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

#include "youtubesearchview.h"

YoutubeSearchView::YoutubeSearchView(QWidget *parent)
	: QListView(parent)
{
	setSelectionMode(QAbstractItemView::MultiSelection);
	setDragEnabled(true);
	setAcceptDrops(false);
//	setObjectName("YoutubeSearchView");
	vScrollBar = verticalScrollBar();
	connect(vScrollBar,SIGNAL(valueChanged(int)),this,SLOT(checkSearchMore(int)));
}

YoutubeSearchView::~YoutubeSearchView()
{

}

void YoutubeSearchView::checkSearchMore(int vBarValue)
{
	qDebug() << "scroll bar value" << vBarValue << "scroll bar max" << vScrollBar->maximum();
	if((vBarValue / vScrollBar->maximum()) * 100 >= 80)
	{
		m_model->searchMore();
		qDebug() << "scroll bar value at 80 %";
	}
}