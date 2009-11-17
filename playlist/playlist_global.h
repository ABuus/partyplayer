#ifndef PLAYLIST_GLOBAL_H
#define PLAYLIST_GLOBAL_H

namespace Playlist {

typedef enum ColumnData 
{
	Artist = 1,
	Title,
	Album,
	Year,
	Track,
	Length,
	Bitrate,
	Place
};

typedef enum ItemRole {
	UrlRole = Qt::UserRole + 1, // contains the url
	PlayRole					// bool true if playing false if not
};

#define ROW_HEIGHT 22

}; // namespace Playlist

#endif //PLAYLIST_GLOBAL_H