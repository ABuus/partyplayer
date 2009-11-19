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
	/* this is not good, try dragging from FF */
	if(event->source()->objectName() == "searchView")
	{
		ytText = event->mimeData()->text();
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
		if(url.toLocalFile().endsWith(".m3u", Qt::CaseInsensitive))
		{
			addM3U(url,row);
			continue;
		}
		QList<QStandardItem *> rowItem;
		PlaylistItem *item = new PlaylistItem(url.toString(), ytText);
		if(!item->isValid())
			continue;
		for(int i = 0; i < m_model->columnCount(); i++)
		{
			QStandardItem *stdItem = new QStandardItem(item->value(i).toString());
			stdItem->setData(url,Qt::UserRole +1 );
			rowItem << stdItem;
		}
		m_model->insertRow(row,rowItem);
		if(m_dragPlaying)
		{
			setPlayRow(row,true);
			setRowHeight(row,ROW_HEIGHT);
			return;
		}
		setRowHeight(row,ROW_HEIGHT);
		row++;
		delete item;
	}
	selectRow(row -1 );
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
	return QUrl();
}

void PlaylistView::addM3U(QUrl url,int row)
{
	QFile file(url.toLocalFile());
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Error: fopen" << url.toString();
		return;
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

			QList<QStandardItem *> rowItem;
			QUrl url("file:///" + filePath + "/" + line);
			PlaylistItem *item = new PlaylistItem(url);
			if(!item->isValid())
			{
				qDebug() << "Error: invalid file" << url;
				continue;
			}
			for(int i = 0; i < m_model->columnCount(); i++)
			{
				QStandardItem *stdItem = new QStandardItem(item->value(i).toString());
				stdItem->setData(url,UrlRole );
				rowItem << stdItem;
			}
			m_model->insertRow(row,rowItem);
			setRowHeight(row,ROW_HEIGHT);
			row++;
			delete item;
		}
	}
	selectRow(row -1 );
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