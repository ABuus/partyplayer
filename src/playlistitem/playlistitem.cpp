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

#include "../mainapp/application.h"
#include "playlistitem.h"

namespace PlaylistItem {

PlaylistItem::PlaylistItem(const QString &location,QObject *parent)
	:QObject(parent),
	m_url(location)
{
	init();
}

PlaylistItem::PlaylistItem(const QUrl &url,QObject *parent)
	:QObject(parent),
	m_url(url.toString())
{
	init();
}

PlaylistItem::~PlaylistItem()
{
}

void PlaylistItem::init()
{
	setValid(false);
	Application *app = (Application*)static_cast<QCoreApplication*>(QApplication::instance());
	Q_ASSERT(app);
	m_loadInfoOnline = app->loadOnlineInfo;
}

QString PlaylistItem::url() const
{
	return m_url;
}

QString PlaylistItem::lengthAsPrittyPrint(uint length)
{
	int min = length / 60;
	int sec = length % 60;
	const QString retval = sec < 10 ? QString("%1:0%2").arg(min).arg(sec) : QString("%1:%2").arg(min).arg(sec);
	return retval;
}

}; // namespace PlaylistItem