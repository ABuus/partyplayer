#ifndef PLAYLISTCONTEXTMENU_H
#define PLAYLISTCONTEXTMENU_H

#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "playlist_export.h"
#include "playlist_global.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistContextMenu : public QMenu
{
	Q_OBJECT

public:
	PlaylistContextMenu(QWidget *parent);
	~PlaylistContextMenu();
	void exec(enum Playlist::InternalData location,const QUrl &url, const QPoint &pos);
private:
	QList<QAction*> menuActions;
	QUrl m_url;
	QAction *openExternal;
private slots:
	void openUrl();
signals:
	void requestRemove();
};

}; // namespace Playlist

#endif // PLAYLISTCONTEXTMENU_H
