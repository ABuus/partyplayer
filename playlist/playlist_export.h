#ifndef PLAYLIST_EXPORT_H
#define PLAYLIST_EXPORT_H
 
#include <QtGlobal>
 
#ifdef PLAYLIST_LIB
    #define PLAYLIST_EXPORT Q_DECL_EXPORT
#else
    #define PLAYLIST_EXPORT Q_DECL_IMPORT
#endif
#endif // PLAYLIST_EXPORT_H