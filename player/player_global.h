#ifndef PLAYER_GLOBAL_H
#define PLAYER_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef PLAYER_LIB
# define PLAYER_EXPORT Q_DECL_EXPORT
#else
# define PLAYER_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAYER_GLOBAL_H
