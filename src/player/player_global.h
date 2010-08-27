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

#ifndef PLAYER_GLOBAL_H
#define PLAYER_GLOBAL_H

#include <Qt/qglobal.h>

#define PLAYER_TIMERINTERVAL 50 // 50 msec playTimer intervals, used to set the current position on streams.

#ifdef PLAYER_LIB
#	define PLAYER_EXPORT Q_DECL_EXPORT
#else
#	define PLAYER_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAYER_GLOBAL_H
