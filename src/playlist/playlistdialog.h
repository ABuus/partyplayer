#ifndef PLAYLISTDIALOG_H
#define PLAYLISTDIALOG_H

#include <QDialog>
#include <QDesktopServices>
#include <QList>
#include <QUrl>
#include <QString>
#include <QFileInfo>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "playlist_export.h"
#include "savedplaylistmodel.h"
#include "ui_playlistdialog.h"

namespace Playlist {

class PLAYLIST_EXPORT PlaylistDialog : public QDialog, public Ui::PlaylistDialog
{
	Q_OBJECT

public:
	PlaylistDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~PlaylistDialog();
	void setUrls(const QList<QUrl> urls);
private:
	bool playlistExists(const QString &name);
	QString absolutePlaylistPath(const QString &name);
	bool warnOverride();
	QString playlistPath();
	QList<QUrl> m_urls;
	SavedPlaylistModel *m_playlistModel;
private slots:
	void savePlaylist();
	void validatePlaylistName(const QString &name);
	void listViewItemClicked(const QModelIndex &index);
};

}; // NAMESPACE Playlist

#endif // PLAYLISTDIALOG_H
