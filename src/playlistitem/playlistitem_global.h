/*
** This file is part of the Party Player application.
** Copyright (C) 2009  Anders Buus
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
