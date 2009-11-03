#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QStandardItemModel>

class PlaylistModel : public QStandardItemModel
{
	Q_OBJECT

public:
	PlaylistModel(QObject *parent);
	~PlaylistModel();

private:
	
};

#endif // PLAYLISTMODEL_H
