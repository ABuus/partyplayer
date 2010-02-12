#include "savedplaylistmodel.h"

using namespace Playlist;

SavedPlaylistModel::SavedPlaylistModel(QObject *parent)
	: QFileSystemModel(parent)
{
	setRootPath( playlistPath());
}

SavedPlaylistModel::~SavedPlaylistModel()
{

}

QString SavedPlaylistModel::playlistPath()
{
	return QString(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/playlists/");
}