#ifndef SAVEDPLAYLISTMODEL_H
#define SAVEDPLAYLISTMODEL_H

#include <QFileSystemModel>
#include <QDesktopServices>
#include <QDir>
#include "playlist_export.h"

namespace Playlist {

class PLAYLIST_EXPORT SavedPlaylistModel : public QFileSystemModel
{
	Q_OBJECT

public:
	SavedPlaylistModel(QObject *parent);
	~SavedPlaylistModel();
	static QString playlistPath();
private:
	
};

}; // NAMESPACE Playlist

#endif // SAVEDPLAYLISTMODEL_H
