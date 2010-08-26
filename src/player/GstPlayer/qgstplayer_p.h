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

#ifndef QGSTPLAYER_P_H
#define QGSTPLAYER_P_H

#include <QDebug>
#include <QUrl>
#include <QEventLoop>
#include <gst/gst.h>
#include <glib.h>
#include "qgstplayer.h"

class QGstPlayerPrivate
{
	Q_DECLARE_PUBLIC(QGstPlayer);
public:
	QGstPlayerPrivate();
	virtual ~QGstPlayerPrivate();
	void init();
	bool playUrl(const QUrl &url);
	bool play();
	bool stop();
	bool pause();
	void seek(const qint64 msec);
	bool enqueue(const QUrl &url);
	void setState(GstState state);
	void emitFinished();
	void updateDuration();
	void updatePosision();
	GstElement *pipeline;
	GstElement *sink;
	QGstPlayer * q_ptr;
	int m_state;
	bool durationSet;
private:
	static gboolean syncEventloop( gpointer ptr );
	static gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer ptr );
	qint64 m_posision;
	qint64 m_duration;
	GMainLoop *gLoop;
};

#endif // QGSTPLAYER_P_H