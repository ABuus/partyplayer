#ifndef SAVEDPLAYLISTMODEL_H
#define SAVEDPLAYLISTMODEL_H

#include <QFileSystemModel>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QMimeData>
#include <QString>
#include "playlist_export.h"

namespace Playlist {

class PLAYLIST_EXPORT SavedPlaylistModel : public QFileSystemModel
{
	Q_OBJECT

public:
	SavedPlaylistModel(QObject *parent);
	~SavedPlaylistModel();
	static QString playlistPath();
	static bool createPlaylistPath();
	QMimeData * mimeData(const QModelIndexList &indexes) const;
private:
	
};

}; // NAMESPACE Playlist

#endif // SAVEDPLAYLISTMODEL_H
