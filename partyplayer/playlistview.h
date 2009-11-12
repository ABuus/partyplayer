#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

//Qt
#include <QTableView>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QTime>
#include <QDir>
#include <QFileInfo>
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
	QObjectList webPages;
public slots:
	QUrl next();
	QUrl previous();
	void selectNext();
	void selectPrevious();
private:
	void insertFile(const QString file, int row);
	void insertDir(const QUrl url,int row);
	void insertYTItem(const QUrl url, const QString title, const int time, int row = -1);
	void mouseDoubleClickEvent( QMouseEvent * event );
	
private slots:
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
signals:
	void playRequest(QUrl url);
	
};

#endif // PLAYLISTVIEW_H
