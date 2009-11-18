#ifndef PLAYLIST_GLOBAL_H
#define PLAYLIST_GLOBAL_H
 
#include <QtGlobal>
#include "playlist_export.h"

namespace Playlist {

#define ROW_HEIGHT 22

	enum PLAYLIST_EXPORT ColunmData {
		Artist = 1,
		Title,
		Album,
		Year,
		Track,
		Length,
		Bitrate,
		Place
	};

	enum PLAYLIST_EXPORT ItemRole {
		UrlRole = Qt::UserRole +1,
		PlayRole
	};

	void Q_DECL_EXPORT registerTypes();

}; // namespace Playlist

#endif // PLAYLIST_GLOBAL_H