#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QStandardItemModel>
#include <QMimeData>
#include <QModelIndex>
#include <QUrl>
#include <QDebug>
#include "playlist_global.h"
#include "playlist_export.h"
#include "playlistitem.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistModel : public QStandardItemModel
{
	Q_OBJECT

public:
	PlaylistModel(QObject *parent);
	~PlaylistModel();
	Qt::DropActions supportedDropActions() const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QStringList mimeTypes() const;
	QMimeData * mimeData(const QModelIndexList &indexes) const;
	void setRowData(int row, const QVariant &value, int role);
private:
	
};

}; // namespace Playlist

#endif // PLAYLISTMODEL_H
