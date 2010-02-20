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
	: QListView(parent),
	searchMoreRequested(false)
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setDragEnabled(true);
	setAcceptDrops(false);
	vScrollBar = verticalScrollBar();
	connect(vScrollBar,SIGNAL(valueChanged(int)),this,SLOT(checkSearchMore(int)));
	
}

YoutubeSearchView::~YoutubeSearchView()
{

}

void YoutubeSearchView::checkSearchMore(int vBarValue)
{
	if(vScrollBar->value() == 0)
		return;
	if((vBarValue / vScrollBar->maximum()) * 100 >= 80 && searchMoreRequested != true)
	{
		searchMoreRequested = true;
		m_model->searchMore();
	}
}

void YoutubeSearchView::onSearchFinished()
{
	searchMoreRequested = false;
}

void YoutubeSearchView::setModel(YoutubeSearchModel *model)
{
	m_model = model;
	QListView::setModel(m_model);
	connect(m_model,SIGNAL(searchFinished()),this,SLOT(onSearchFinished()));
}