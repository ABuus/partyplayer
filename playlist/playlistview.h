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
	void addM3U(QUrl url, int row);
	void setPlayRow(int row, bool playing = false);
	PlaylistModel *m_model;
	QPoint startDragPos;
	int m_playRow;
	bool m_dragPlaying;
protected:
	void dragMoveEvent(QDragMoveEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void dropEvent(QDropEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
};

}; // namespace Playlist

#endif // PLAYLIST_H
