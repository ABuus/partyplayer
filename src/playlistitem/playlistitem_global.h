#ifndef PLAYLISTITEM_GLOBAL_H
#define PLAYLISTITEM_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef PLAYLISTITEM_LIB
# define PLAYLISTITEM_EXPORT Q_DECL_EXPORT
#else
# define PLAYLISTITEM_EXPORT Q_DECL_IMPORT
#endif

namespace PlaylistItem {

/*
 * on windows path is file:///C:/...
 * on linux path is file://home/...
 */
#ifdef Q_WS_WIN
#define ROOT_PATH "/"
#else
#define ROOT_PATH ""
#endif
#define FILE_MARCO "file://" ROOT_PATH

}; // namespace PlaylistItem

#endif // PLAYLISTITEM_GLOBAL_H
