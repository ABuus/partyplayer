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
	m_dragRow(-1),
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
	setDropIndicatorShown(false);
	setDragDropMode(QAbstractItemView::DragDrop);
	setItemDelegate(new PlaylistDelegate(this));
	verticalHeader()->setResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(20);
	verticalHeader()->setHighlightSections(false);
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setStretchLastSection(false);
	setColumnHidden(0,true);
}

PlaylistView::~PlaylistView()
{

}

void PlaylistView::dragMoveEvent(QDragMoveEvent *event)
{
	m_dragRow = indexAt(event->pos()).row();
	update();
}

void PlaylistView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton && selectedIndexes().count() > 0)
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
	/* find the right row */
	int row = indexAt(event->pos()).row();
	Debug << row;
	if(row == -1)
	{
		row = m_model->rowCount();
	}
	else if(row > indexAt(startDragPos).row())
	{
		++row;
	}
	
	/* insert files */
	QList<QUrl> urls = event->mimeData()->urls();
	Debug << urls << row;
	foreach(QUrl url, urls)
	{
		if(url.scheme() == "file")
		{
			addFile(url.toString(),row++);
			if(m_dragPlaying)
				setPlayRow(row -1);
			continue;
		}
		else if(url.host().contains("youtube.com"))
		{
			if(row > indexAt(startDragPos).row())
			{
				--row;
			}
			addYoutube(url,row++);
			if(m_dragPlaying)
				setPlayRow(row -1);
			continue;
		}
		else
		{
			// unsuported content
			continue;
		}
	}
	selectRow(row);
}

void PlaylistView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() != Qt::LeftButton)
		return;
	if(m_playRow != -1)
	{
		setPlayRow(m_playRow,false);
	}
	QModelIndex index = indexAt(event->pos());
	QVariant data = m_model->data(index,UrlRole);
	playRequest(data);

	if(m_playRow == index.row())
		return;
	setPlayRow(index.row(), true);
}

QVariant PlaylistView::next()
{
	QStandardItem *item = m_model->item(m_playRow +1);
	if(item == 0)
		return QUrl();
	QVariant data = item->data(UrlRole);
	setPlayRow(m_playRow +1,true);
	Debug << "play next";
	return data;
}

QVariant PlaylistView::previous()
{
	QStandardItem *item = m_model->item(m_playRow -1);
	if(m_playRow == 0 || m_playRow == -1)
		return QUrl();
	QVariant data = item->data(UrlRole);
	setPlayRow(m_playRow -1,true);
	Debug << "play previous";
	return data;
}

bool PlaylistView::addM3U(const QString file,int row)
{
	QFile m_file(file);
	if(!m_file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Debug << "Error: fopen" << file;
		return false;
	}
	while(!m_file.atEnd())
	{
		QByteArray line = m_file.readLine();
		if(line.startsWith("#"))
			continue;
		else
		{
			QString filePath = file;
			filePath.remove(file.lastIndexOf("/"),filePath.size());
			if(line.endsWith("\n"))
				line.chop(1);
			QUrl url("file:///" + filePath + "/" + line);
			addFile(url.toString(),row++);
			Debug << "M3U insert url" << url;
		}
	}
	selectRow(row -1 );
	return true;
}

void PlaylistView::setPlayRow(int row, bool playing)
{
	if(row == -1 && playing == false)
	{
		m_playRow = -1;
		return;
	}
	for(int i = m_model->columnCount() -1 ; i > 0; i--)
	{
		m_model->item(row,i)->setData(true, PlayRole);
		update(m_model->index(row,i));
	}
	if(playing && m_playRow != -1)
	{
		for(int i = m_model->columnCount() -1 ; i > 0; i--)
		{
			m_model->item(m_playRow,i)->setData(false, PlayRole);
			update(m_model->index(m_playRow,i));
		}
		m_playRow = row;
	}
	else if(playing)
	{
		m_playRow = row;
	}
}

bool PlaylistView::addFile(QString &file, int row)
{
	if(row == -1 || row > m_model->rowCount())
	{
		row = m_model->rowCount();
	}
	if(file.endsWith(".m3u", Qt::CaseInsensitive))
	{
		return addM3U(file,row);
	}
	else
	{
		if(!file.startsWith("file:///"))
			file.prepend("file:///");
		QList<QStandardItem *> rowItem;
		PlaylistItem *item = new PlaylistItem(file,row,this);
		connect(item,SIGNAL(dataRecived(int)),this,SLOT(handleItemData(int)));
		if(!item->isValid())
		{
			if(addDir(file,row))
				return true;
			return false;
		}
		for(int i = 0; i < m_model->columnCount(); i++)
		{
			QString value = item->value(i).toString();
			QStandardItem *stdItem = new QStandardItem(value);
			stdItem->setData(file,UrlRole);
			rowItem << stdItem;
		}
		Debug << "insert data row & data:" << row << rowItem;
		m_model->insertRow(row,rowItem);
		delete item;
		return true;
	}
	return false;
}

bool PlaylistView::addYoutube(const QUrl url, int row)
{
	if(row == -1 || row > m_model->rowCount())
	{
		row = m_model->rowCount();
	}
	PlaylistItem *item = new PlaylistItem(url,row,this);
	connect(item,SIGNAL(dataRecived(int)),this,SLOT(handleItemData(int)));

	return true;
}

bool PlaylistView::addYoutube(const QString title,const QString description, 
							  const QString vidId, const QString duration, int row)
{
	if(row == -1)
	{
		row = m_model->rowCount();
	}
	QList<QStandardItem *> rowItem;

	/* convert strings to QStandardItems */
	QStandardItem *i_title = new QStandardItem(title);
	QStandardItem *i_description = new QStandardItem(description);
	QStandardItem *i_vidId = new QStandardItem(vidId);
	QStandardItem *i_duration = new QStandardItem(duration);

	/* empty placeholder items */
	QStandardItem *placeholder_1 = new QStandardItem("");
	QStandardItem *placeholder_2 = new QStandardItem("");
	QStandardItem *placeholder_3 = new QStandardItem("");
	QStandardItem *placeholder_4 = new QStandardItem("");

	/* insert items into row */
	rowItem << i_title << placeholder_1 << placeholder_2 
		<< placeholder_3 << i_description << i_duration << placeholder_4 << i_vidId;
	foreach(QStandardItem *item, rowItem)
	{
		item->setData(vidId,UrlRole);
	}
	m_model->insertRow(row,rowItem);
	setRowHeight(row,ROW_HEIGHT);
	return true;
}

void PlaylistView::paintEvent(QPaintEvent *event)
{
	if(m_dragRow != -1)
	{
		// this is not implemented
	}
	QTableView::paintEvent(event);
}

void PlaylistView::clear()
{
	model()->removeRows(0,model()->rowCount());
	setPlayRow(-1);
}

bool PlaylistView::addDir(QString path, int row)
{
	Debug << "Dir path: " << path << row;
	path.remove("file:///");
	QDir dir;
	dir.setPath(path);
	if(!dir.exists())
	{
		Debug << "nonexisting dir" << path;
		return false;
	}
	QStringList entryList;
	entryList << dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files,QDir::DirsFirst);
	foreach(QString entry, entryList)
	{
		if(!entry.endsWith("m3u",Qt::CaseInsensitive))
			if(!addFile( QString("file:///" + path + "/" + entry) ,row++))
				row--;
	}
	return true;
}

void PlaylistView::handleItemData(int row)
{
	PlaylistItem *item = qobject_cast<PlaylistItem*>(sender());
	if(row == -1)
		item->deleteLater();
	else
	{
		QList<QStandardItem *> rowItem;
		for(int i = 0; i < m_model->columnCount(); i++)
		{
			QString value = item->value(i).toString();
			QStandardItem *stdItem = new QStandardItem(value);
			stdItem->setData(item->value(PlaylistItem::Place),UrlRole);
			rowItem << stdItem;
		}
		Debug << "insert data row & data:" << row << rowItem;
		m_model->insertRow(row,rowItem);
	}
	item->deleteLater();
}