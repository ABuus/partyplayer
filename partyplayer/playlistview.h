#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

//Qt
#include <QTableView>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
// taglib
#include <fileref.h>
#include <tag.h>
#include <id3v2tag.h>
// My
#include "playlistmodel.h"
#include "debug.h"

class PlaylistView : public QTableView
{
	Q_OBJECT

public:
	PlaylistView(QWidget *parent);
	~PlaylistView();

private:
	PlaylistModel *model;
	void insetItem(QStringList item);
	void mouseDoubleClickEvent( QMouseEvent * event );
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
signals:
	void playRequest(QUrl url, bool localFile);
	
};

#endif // PLAYLISTVIEW_H
