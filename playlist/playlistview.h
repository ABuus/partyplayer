#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QTableView>
#include <QDragMoveEvent>
#include <QUrl>
#include <QPoint>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include "playlist_global.h"
#include "playlist_export.h"
#include "playlistmodel.h"
#include "playlistitem.h"
#include "playlistdelegate.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistView : public QTableView
{
	Q_OBJECT
public:
	PlaylistView(QWidget *parent = 0);
	~PlaylistView();
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

}; // namespace Playlist

#endif // PLAYLIST_H
