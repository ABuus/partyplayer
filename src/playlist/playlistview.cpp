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
#include <QScrollBar>

using namespace Playlist;

PlaylistView::PlaylistView(QWidget *parent)
	: QTreeView(parent)
{
	// setup this
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::ContiguousSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(false);
	setDragDropMode(QAbstractItemView::DragDrop);
//	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setSortingEnabled(true);
	setRootIsDecorated(false);
	setMouseTracking(true);
	/* we let the delegate take care of that */
	setIndentation(0);
	setAnimated(true);
	setExpandsOnDoubleClick(false);
//	setAlternatingRowColors(true);

	contexMenu = new PlaylistContextMenu(this);

	// delegate
	m_delegate = new PlaylistDelegate(this);
	setItemDelegate(m_delegate);
	
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
	connect(contexMenu,SIGNAL(requestRemove()),this,SLOT(removeSelected()));
}

/* 
 * force model to be Playlist::playlistModel 
 */

void PlaylistView::setModel(Playlist::PlaylistModel *model)
{
	m_model = model;
	QTreeView::setModel(m_model);
	connect(m_model,SIGNAL(rowsInserted(const QModelIndex &,int,int)),this,SLOT(setChildSpanned(const QModelIndex &,int)));
}

/*
 * get the url after the item that is in play state
 * if set is true (the default) play role is set at next row
 */

QUrl PlaylistView::next(bool set)
{
//	collapseAll();
	QUrl retval;
	int currentRow = m_model->getPlayRow() + 1;
	if(currentRow <= 0 || currentRow > m_model->rowCount())
		retval = QUrl();
	else
		retval = m_model->index(currentRow,0).data(UrlRole).toUrl();
	if(set)
	{
//		expand(m_model->index(currentRow,0));
		m_model->setPlayRow(currentRow);
	}
	return retval;
}

/*
 * get the url before the item that is in play state
 * if set is true (the default) play role is set at previous row
 */

QUrl PlaylistView::previous(bool set)
{
//	collapseAll();
	QUrl retval;
	int currentRow = (m_model->getPlayRow() - 1);
	if(currentRow < 0 )
		retval = QUrl();
	else
		retval = m_model->index(currentRow,0).data(UrlRole).toUrl();
	if(set)
	{
//		expand(m_model->index(currentRow,0));
		m_model->setPlayRow(currentRow);
	}
	return retval;
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
	/* set drag urls and execute the drag */
	mimeData->setUrls(urls);
	drag->setMimeData(mimeData);
	drag->exec();
}

/*
 * Handle internal move or send the event to parent class.
 */

void PlaylistView::dropEvent(QDropEvent *event)
{
	event->accept();
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
		QList< QList<QStandardItem*> > rowList;
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
//	QTableView::dropEvent(event);
	m_model->dropMimeData(event->mimeData(),Qt::CopyAction,indexAt(event->pos()).row(),-1,QModelIndex());
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
	clearSelection();
}

void PlaylistView::mouseMoveEvent(QMouseEvent *event)
{
	QTreeView::mouseMoveEvent(event);
	int element = extendedElementAt(event->pos());
	if(!element)
	{
		unsetCursor();
		return;
	}
	else
	{
		if(indexAt(event->pos()).parent().isValid() && element == HandleElement)
		{
			return;
		}
		setCursor(Qt::PointingHandCursor);
	}
	event->accept();
}

void PlaylistView::contextMenuEvent(QContextMenuEvent *event)
{
	QModelIndex index = indexAt(event->pos());
	if(!index.isValid())
		return;
	Playlist::InternalData type = (Playlist::InternalData)index.data(Playlist::PlacementRole).toInt();
	const QUrl url = index.data(Playlist::UrlRole).toUrl();
	contexMenu->exec(type,url,mapToGlobal(event->pos()));
}

void PlaylistView::removeSelected()
{
	QList<QModelIndex> indexes = selectedIndexes();
	QList<int> rows;
	foreach(QModelIndex index,indexes)
	{
		if(!rows.contains(index.row()))
		{
			rows.append(index.row());
		}
	}
	while(!rows.isEmpty())
	{
		m_model->removeRow(rows.takeLast());
	}
}

void PlaylistView::setChildSpanned(const QModelIndex &, int start)
{
	/* merge celles of child indexes */
	QModelIndex index = model()->index(start,0);
	setFirstColumnSpanned(0,index,true);
}

void PlaylistView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() != Qt::LeftButton)
	{
		QTreeView::mousePressEvent(event);
		return;
	}

	const QModelIndex index = indexAt(event->pos());
	int element = extendedElementAt(event->pos());

	switch (element) {
		case NoElement:
			break;
		case UrlElement:
			openExternalUrl(index);
			break;
		case HandleElement:
			if(isExpanded(index))
				collapse(index);
			else
				expand(index);
			break;
		default:
			break;
	}
	QTreeView::mousePressEvent(event);
}

int PlaylistView::extendedElementAt(const QPoint &point)
{
	const QModelIndex index = indexAt(point);
	QRect indexRect = visualRect(index);
	const QRectF handleRect = m_delegate->extendedHandleRect();
	QPoint pressedOffset = point;
	pressedOffset.setY( pressedOffset.y() - indexRect.y());
	
	/* handle */
	if(handleRect.contains(pressedOffset))
	{
		return HandleElement;
	}

	/* only handle have parent index */
	if(!index.parent().isValid())
	{
		return NoElement;
	}

	/* url */
	return m_delegate->extendedUrl(indexRect.width(),pressedOffset) ? UrlElement : NoElement;
}

void PlaylistView::openExternalUrl(const QModelIndex &index)
{
	QString urlString = index.data(Playlist::UrlRole).toString();
	if(index.data(Playlist::PlacementRole).toInt() == Playlist::Local)
	{
		urlString.remove(urlString.lastIndexOf("/"),urlString.size());
	}
	Debug << urlString;
	QDesktopServices::openUrl(QUrl(urlString));
}