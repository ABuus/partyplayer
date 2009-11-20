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
	: QTableView(parent),
	m_playRow(-1),
	m_dragPlaying(false)
{
	// model
	m_model = new PlaylistModel(this);
	setModel(m_model);
	// view
	setShowGrid(false);
	setSortingEnabled(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setItemDelegate(new PlaylistDelegate(this));
}

PlaylistView::~PlaylistView()
{

}

void PlaylistView::dragMoveEvent(QDragMoveEvent *event)
{
	event->accept();
}

void PlaylistView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
		startDragPos = event->pos();
	QTableView::mousePressEvent(event);
}

void PlaylistView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint moved = event->pos() - startDragPos;
	if(moved.manhattanLength() < 3)
	{
		QTableView::mouseMoveEvent(event);
		return;
	}
	QList<QUrl> urls;
	QModelIndex index = indexAt(startDragPos);
	QStandardItem *item = m_model->itemFromIndex(index);
	if(item->data(PlayRole).toBool())
		m_dragPlaying = true;
	urls << model()->data(index,Qt::UserRole + 1).toUrl();
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;
	mimeData->setUrls(urls);
	drag->setMimeData(mimeData);

	if(drag->exec() == Qt::MoveAction)
	{
		QModelIndex oldIndex = m_model->indexFromItem(item);
		m_model->removeRow(oldIndex.row());
	}
	m_dragPlaying = false;
}

void PlaylistView::dropEvent(QDropEvent *event)
{
	QString ytText;
	if(!event->mimeData()->hasUrls())
		event->ignore();
	/* this is not good, try dragging from FF */
	if(event->mimeData()->hasFormat("application/yt-partyplayer"))
	{
		ytText = event->mimeData()->data("application/yt-partyplayer");
	}
	int row = indexAt(event->pos()).row();
	if(row == -1)
	{
		row = m_model->rowCount();
	}
	else if(row > indexAt(startDragPos).row())
	{
		++row;
	}
	QList<QUrl> urls = event->mimeData()->urls();
	foreach(QUrl url, urls)
	{
		if(url.scheme() == "file")
		{
			addFile(url.toString(),row);
			continue;
		}
		else if(url.scheme() == "http")
		{
			addUrl(url,ytText,row);
			continue;
		}
		/* addDirectory() */
		else
		{
			
			// unsuported
			continue;
		}
	}
	selectRow(row);
}

void PlaylistView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(m_playRow != -1)
	{
		setPlayRow(m_playRow,false);
	}
	QModelIndex index = indexAt(event->pos());
	playRequest(m_model->data(index,UrlRole).toUrl());
	setPlayRow(index.row(), true);
}

QUrl PlaylistView::next()
{
	QStandardItem *item = m_model->item(m_playRow +1);
	if(item == 0)
		return QUrl();
	QUrl url = item->data(UrlRole).toUrl();
	setPlayRow(m_playRow +1,true);
	qDebug() << "play next";
	return url;
}

QUrl PlaylistView::previous()
{
	QUrl url = m_model->item(m_playRow -1)->data(UrlRole).toUrl();
	setPlayRow(m_playRow -1,true);
	qDebug() << "play previous";
	return url;
}

bool PlaylistView::addM3U(QUrl url,int row)
{
	QFile file(url.toLocalFile());
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Error: fopen" << url.toString();
		return false;
	}
	while(!file.atEnd())
	{
		QByteArray line = file.readLine();
		if(line.startsWith("#"))
			continue;
		else
		{
			QString filePath = url.toLocalFile();
			filePath.remove(filePath.lastIndexOf("/"),filePath.size());
			if(line.endsWith("\n"))
				line.chop(1);
			QUrl url("file:///" + filePath + "/" + line);
			addFile(url.toString(),++row);
		}
	}
	selectRow(row -1 );
	return true;
}

void PlaylistView::setPlayRow(int row, bool playing)
{
	qDebug() << "setting row:" << row << "to PlayState" << playing;
	for(int i = 0; i < m_model->columnCount(); i++)
	{
		m_model->item(row,i)->setData(playing, PlayRole);
	}
	if(playing && m_playRow != -1)
	{
		for(int i = 0; i < m_model->columnCount(); i++)
		{
			m_model->item(m_playRow,i)->setData(false, PlayRole);
		}
		m_playRow = row;
	}
	else if(playing)
	{
		m_playRow = row;
	}
}

bool PlaylistView::addFile(const QString &file, int row)
{
	QUrl url(file);
	if(row == -1)
	{
		row = m_model->rowCount();
	}
	if(url.toLocalFile().endsWith(".m3u", Qt::CaseInsensitive))
	{
		return addM3U(url,row);
	}
	else
	{
		QList<QStandardItem *> rowItem;
		PlaylistItem *item = new PlaylistItem(url);
		if(!item->isValid())
		{
			qDebug() << "Error: invalid file" << url;
			return false;
		}
		for(int i = 0; i < m_model->columnCount(); i++)
		{
			QStandardItem *stdItem = new QStandardItem(item->value(i).toString());
			stdItem->setData(url,UrlRole );
			rowItem << stdItem;
		}
		m_model->insertRow(row,rowItem);
		setRowHeight(row,ROW_HEIGHT);
		delete item;
		return true;
	}
	return false;
}

bool PlaylistView::addUrl(QUrl url, QString ytText, int row)
{
	if(row == -1)
	{
		row = m_model->rowCount();
	}
	QList<QStandardItem *> rowItem;
	PlaylistItem *item = new PlaylistItem(url,ytText);
	if(!item->isValid())
	{
		qDebug() << "Error: invalid file" << url;
		return false;
	}
	for(int i = 0; i < m_model->columnCount(); i++)
	{
		QStandardItem *stdItem = new QStandardItem(item->value(i).toString());
		stdItem->setData(url,UrlRole );
		rowItem << stdItem;
	}
	m_model->insertRow(row,rowItem);
	setRowHeight(row,ROW_HEIGHT);
	delete item;
	return true;
}