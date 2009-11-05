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
	PlaylistModel *model;
public slots:
	QUrl next();
	QUrl previous();
//	void checkCurrentIndex(const QUrl &);
	void selectNext();
	void selectPrevious();
private:
	void insetItem(QStringList item);
	void mouseDoubleClickEvent( QMouseEvent * event );
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
signals:
	void playRequest(QUrl url);
	
};

#endif // PLAYLISTVIEW_H
