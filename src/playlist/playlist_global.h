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

#ifndef PLAYLIST_GLOBAL_H
#define PLAYLIST_GLOBAL_H
 
#include <QtGlobal>
#include "playlist_export.h"

namespace Playlist {

#define ROW_HEIGHT 22

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

	enum PLAYLIST_EXPORT ColunmData {
		Artist = 0,
		Title,
		Album,
		Year,
		Track,
		Length,
		Bitrate,
		Place,
		Internal
	};

	enum PLAYLIST_EXPORT ItemRole {
		UrlRole = Qt::UserRole +1,
		PlayRole,
		PlacementRole,
		ValidRole,
		HoverRole,
		ExtendedDataImage,
		ExtendedDataDescription,
		ExtendedDataYear,
		ExtendedDataBitrate,
		ExtendedDataSampleRate
	};

	enum PLAYLIST_EXPORT InternalData {
		Local = 0,
		Youtube
	};

	void Q_DECL_EXPORT registerTypes();

}; // namespace Playlist

#endif // PLAYLIST_GLOBAL_H