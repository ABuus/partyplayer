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

#ifndef QGSTPLAYER_H
#define QGSTPLAYER_H

#include <QThread>
#include <QUrl>
#include "player_global.h"

class QGstPlayerPrivate;

class PLAYER_EXPORT QGstPlayer : public QObject
{
	Q_OBJECT
public:
	explicit QGstPlayer(QObject *parent = 0);
	~QGstPlayer();
	void run();
public slots:
	bool playUrl(const QUrl &url);
	bool stop();
	bool play();
	bool pause();
	bool enqueueNext(const QUrl &url);
	void seek(const qint64 msec);
signals:
	/** This signal is emmitted when the the position in the stream changes. */
	void timeChanged(qint64 time);
	/** This signal is emmitted when the duration of a stream changes */
	void durationChanged(qint64 time);
	/** This signal is emmitted when the player changes state. */
	void stateChanged( int state );
	/** This signal is emmitted just before the current stream ends */
	void runningOut();
protected:  
	QGstPlayerPrivate * const d_ptr;
	QGstPlayer(QGstPlayerPrivate &dd, QObject * parent);
private:
	Q_DECLARE_PRIVATE(QGstPlayer);
};

#endif // QGSTPLAYER_H
