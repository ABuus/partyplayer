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

#include "qgstplayer.h"
#include "qgstplayer_p.h"

QGstPlayer::QGstPlayer(QObject *parent)
	: QThread(parent),
	d_ptr(new QGstPlayerPrivate())
{
	Q_D(QGstPlayer);
	d->q_ptr = this;
}

QGstPlayer::QGstPlayer(QGstPlayerPrivate &dd, QObject *parent)
	: QThread(parent),
	d_ptr(&dd)
{
	Q_D(QGstPlayer);
	d->q_ptr = this;
}

QGstPlayer::~QGstPlayer()
{
}

void QGstPlayer::run()
{
	Q_D(QGstPlayer);
	d->init();
}

bool QGstPlayer::playUrl(const QUrl &url)
{
	Q_D(QGstPlayer);
	return d->playUrl(url);
}

bool QGstPlayer::play()
{
	Q_D(QGstPlayer);
	return d->play();
}
bool QGstPlayer::pause()
{
	Q_D(QGstPlayer);
	return d->pause();
}

bool QGstPlayer::stop()
{
	Q_D(QGstPlayer);
	return d->stop();
}

void QGstPlayer::seek(const qint64 msec)
{
	Q_D(QGstPlayer);
	d->seek(msec);
}

bool QGstPlayer::enqueueNext(const QUrl &url)
{
	Q_D(QGstPlayer);
	return d->enqueue(url);
}
