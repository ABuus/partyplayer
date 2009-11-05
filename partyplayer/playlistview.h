#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

//Qt
#include <QTableView>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QTime>
// taglib
#include <fileref.h>
#include <tag.h>
#include <audioproperties.h>
// My
#include "playlistmodel.h"
#include "debug.h"

class PlaylistView : public QTableView
{
	Q_OBJECT

public:
	PlaylistView(QWidget *parent);
	~PlaylistView();
public slots:
	void findNext();
	void checkCurrentIndex(const QUrl &);
private:
	PlaylistModel *model;
	void insetItem(QStringList item);
	void mouseDoubleClickEvent( QMouseEvent * event );
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
signals:
	void playRequest(QUrl url, bool localFile);
	void nextRequest(QUrl url, bool localFile);
	
};

#endif // PLAYLISTVIEW_H
