#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QListView>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include "playlistmodel.h"
#include "debug.h"

class PlaylistView : public QListView
{
	Q_OBJECT

public:
	PlaylistView(QWidget *parent);
	~PlaylistView();

private:
	PlaylistModel *model;
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	
};

#endif // PLAYLISTVIEW_H
