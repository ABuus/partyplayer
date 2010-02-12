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

#ifndef PLAYER_H
#define PLAYER_H

// Qt
#include <QObject>
#include <QUrl>
#include <QTimer>
// gstreamer
#include <glib-object.h>
#include <gst/gst.h>

#include "player_global.h"
#include "debug.h"

#define TIMER_INTERVAL 200

class PLAYER_EXPORT LocalPlayer : public QObject
{
	Q_OBJECT
public:
	LocalPlayer(QObject *parent = 0);
	~LocalPlayer();
public slots:
	void playUrl(const QUrl &url);
	void seek( int time );
	void play();
	void stop();
	void pause();
	bool enqueue(const QUrl &url);
signals:
	void timeChanged( qint64 pos );
	void totalTimeChanged( qint64 totalTime );
	void runningOut();
	void stateChanged( int state );
private:
	GstElement *m_pipeline;
	GstElement *m_newPipeline;
	GstElement *m_sink;
	bool m_canRunOut;
	QTimer m_playTimer;
	qint64 m_totaltime;
	bool m_totalTimeSet;
	int m_state; // 0=stop,1=playing,2=paused
private slots:
	void getTime();
	void getTotalTime();
	GstElement *createPipeline();
	void checkState();
};

#endif // PLAYER_H