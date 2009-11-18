#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include "playlist_global.h"

namespace Playlist {

class PlaylistDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	PlaylistDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}; // namespace Playlist

#endif // PLAYLISTDELEGATE_H
