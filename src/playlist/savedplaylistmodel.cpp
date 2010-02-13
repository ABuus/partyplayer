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

QMimeData * SavedPlaylistModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QList<QUrl> urls;
	foreach(QModelIndex index, indexes)
	{
		QString fileName(playlistPath());
		fileName.append(index.data().toString());
		QFile file(fileName);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return 0;
		while(!file.atEnd())
		{
			const QString line = file.readLine().data();
			urls.append( QUrl(line.simplified()) );
		}
		file.close();
	}
	mimeData->setUrls(urls);
	return mimeData;
}

bool SavedPlaylistModel::createPlaylistPath()
{
	QDir dir;
	return dir.mkpath(SavedPlaylistModel::playlistPath());
}