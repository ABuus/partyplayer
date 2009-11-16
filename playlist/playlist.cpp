#include "playlist.h"

Playlist::Playlist(QWidget *parent)
	: QTableView(parent)
{
	m_model = new PlaylistModel(this);
	setModel(m_model);
	setShowGrid(false);
	setSortingEnabled(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::DragDrop);
}

Playlist::~Playlist()
{

}

void Playlist::dragMoveEvent(QDragMoveEvent *event)
{
	event->accept();
}

void Playlist::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
		startDragPos = event->pos();
	QTableView::mousePressEvent(event);
}

void Playlist::mouseMoveEvent(QMouseEvent *event)
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
	urls << model()->data(index,Qt::UserRole).toUrl();
	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;
	mimeData->setUrls(urls);
	drag->setMimeData(mimeData);
	if(drag->exec() == Qt::MoveAction)
	{
		QModelIndex newIndex = m_model->indexFromItem(item);
		m_model->removeRow(newIndex.row());
	}
}

void Playlist::dropEvent(QDropEvent *event)
{
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
		QList<QStandardItem *> rowItem;
		PlaylistItem *item = new PlaylistItem(url.toString());
		for(int i = 0; i < m_model->columnCount(); i++)
		{
			QStandardItem *stdItem = new QStandardItem(item->value(i).toString());
			stdItem->setData(url,Qt::UserRole);
			rowItem << stdItem;
		}
		m_model->insertRow(row,rowItem);
		row++;
		delete item;
	}
	selectRow(row -1 );
}

void Playlist::mouseDoubleClickEvent(QMouseEvent *event)
{
	playRequest(m_model->data(indexAt(event->pos()),Qt::UserRole).toUrl());
}