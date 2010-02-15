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

#include "playlistview.h"

using namespace Playlist;

PlaylistView::PlaylistView(QWidget *parent)
	: QTableView(parent)	
{
	// setup this
	setShowGrid(false);
	setSortingEnabled(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::ContiguousSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(false);
	setDragDropMode(QAbstractItemView::DragDrop);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	verticalHeader()->setResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(20);
	verticalHeader()->setHighlightSections(false);
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setStretchLastSection(false);
	setColumnHidden(Playlist::Internal,true);
//	viewport()->setAttribute(Qt::WA_Hover);
	setMouseTracking(true);

	// delegate 
	setItemDelegate(new PlaylistDelegate(this));
	
	// connections
	createConnections();
}

PlaylistView::~PlaylistView()
{

}

/*
 * connect Signals & Slots
 */

void PlaylistView::createConnections()
{
	connect(this,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(onDoubleClicked(const QModelIndex &)));
}

/* 
 * force model to be Playlist::playlistModel 
 */

void PlaylistView::setModel(Playlist::PlaylistModel *model)
{
	m_model = model;
	QTableView::setModel(m_model);
}

/*
 * get the url after the item that is in play state
 */

QUrl PlaylistView::next()
{
	Debug << "UNIMPLEMENTED";
	return QUrl();
}

/*
 * get the url before the item that is in play state
 */

QUrl PlaylistView::previous()
{
	Debug << "UNIMPLEMENTED";
	return QUrl();
}

/*
 * clear all data from Playlist::PlaylistView
 * BUG: this is data handling and should be moved to Playlist::PlaylistModel
 */

void PlaylistView::clear()
{
	model()->removeRows(0,model()->rowCount());
}

/*
 * Shows a Playlist::PlaylistDialog whers the user can save a playlist
 */

void PlaylistView::save()
{
	QList<QUrl> urls;
	for(int i = 0; i < m_model->rowCount(); i++)
	{
		QUrl url(m_model->data( m_model->index(i,0), UrlRole).toUrl());
		urls << url;
	}
	PlaylistDialog *dialog = new PlaylistDialog(this);
	dialog->setUrls(urls);
	dialog->exec();
	dialog->deleteLater();
}

/*
 * Gather urls and execute a drag event
 */

void PlaylistView::startDrag(Qt::DropActions supportenDropActions)
{
	Q_UNUSED(supportenDropActions);

	QList<QModelIndex> indexes = selectedIndexes();
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData();
	QList<QUrl> urls;
	/* get urls from indexex */
	foreach(QModelIndex index,indexes)
	{
		const QUrl url = index.data(Playlist::UrlRole).toUrl();
		if(!urls.contains(url))
			urls.append(url);
	}
	/* set drag urls and execute eth drag */
	mimeData->setUrls(urls);
	drag->setMimeData(mimeData);
	drag->exec();
}

/*
 * Handle internal move or send the event to parent class.
 */

void PlaylistView::dropEvent(QDropEvent *event)
{
	/* internal move */
	if(event->source() == this)
	{
		/*
		 * Collect indexes and there rows 
		 */
		QList<QModelIndex> indexes = selectedIndexes();
		QList<int> rows;
		foreach(QModelIndex index,indexes)
		{
			if(!rows.contains(index.row()))
			{
				rows << index.row();
			}
		}

		/* 
		 * Remove items from model bottom up so we do not skrew up the order, 
		 * insert them into a list so we can reinsert them
		 */
		QList<QList<QStandardItem*>> rowList;
		for(int i = (rows.size() -1); i >= 0; i--)
		{
			rowList.prepend(m_model->takeRow(rows.at(i)));
		}
		
		/* 
		 * Check if the drop should be appended or inserted 
		 */
		int row = indexAt(event->pos()).row();
		if(row == -1)
			row = model()->rowCount();

		/* 
		 * Insert items from list and return 
		 */
		foreach(QList<QStandardItem*> rowItem, rowList)
		{
			m_model->insertRow(row++,rowItem);
		}
		return;
	}
	/* Let the parent class handle the event */
	QTableView::dropEvent(event);
}

/*
 * Set the PlayRole at index to true, and reset old playrole.
 * Get the url from the model and emit playrequest
 */

void PlaylistView::onDoubleClicked(const QModelIndex &index)
{
	m_model->setPlayRow(index.row());
	const QUrl url = model()->data(index,UrlRole).toUrl();
	if(!url.isValid())
		return;
	emit playRequest(url);
}

void PlaylistView::mouseMoveEvent(QMouseEvent *event)
{
	int row = indexAt(event->pos()).row();
	if(row == m_hoverRow)
		return;
	for(int i = 0; i < model()->columnCount(); i++)
	{
		QModelIndex newIndex = m_model->index(row,i);
		QModelIndex oldIndex = m_model->index(m_hoverRow,i);
		if(newIndex.isValid())
		{
			m_model->setData(newIndex,true,Qt::UserRole +5);
			if( i == ( model()->columnCount() -1 ) )
				m_hoverRow = row;
		}
		if(oldIndex.isValid())
		{
			m_model->setData(oldIndex,false,Qt::UserRole +5);
		}
	}
	QTableView::mouseMoveEvent(event);
}