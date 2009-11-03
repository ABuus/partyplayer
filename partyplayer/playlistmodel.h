#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QStandardItemModel>
#include <QMimeData>
#include <QStringList>
#include <QUrl>
#include "debug.h"

class PlaylistModel : public QStandardItemModel
{
	Q_OBJECT

public:
	PlaylistModel(QObject *parent);
	~PlaylistModel();

private:
	void addItem(QStandardItem item);
protected:
	Qt::DropActions supportedDropActions() const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QStringList mimeTypes() const;
	QMimeData * mimeData(const QModelIndexList &indexes) const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
};

#endif // PLAYLISTMODEL_H
