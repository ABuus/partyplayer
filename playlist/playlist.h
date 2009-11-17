#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QTableView>
#include <QDragMoveEvent>
#include <QUrl>
#include <QPoint>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include "playlistmodel.h"
#include "playlistitem.h"

class Playlist : public QTableView
{
	Q_OBJECT
public:
	Playlist(QWidget *parent = 0);
	~Playlist();
	QUrl next();
signals:
	void playRequest(const QUrl url);
private:
	PlaylistModel *m_model;
	QPoint startDragPos;
	void addM3U(QUrl url, int row);
protected:
	void dragMoveEvent(QDragMoveEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void dropEvent(QDropEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // PLAYLIST_H
