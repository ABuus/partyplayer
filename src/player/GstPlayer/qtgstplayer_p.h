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

#include "qtgstplayer.h"
#include <qobject.h>
#include <qurl.h>
#include <qtimer.h>

#include <gst/gst.h>
#include <glib.h>

class QtGstPlayerPrivate : public QObject
{
        Q_OBJECT
        Q_DECLARE_PUBLIC(QtGstPlayer);
public:
        QtGstPlayerPrivate(QtGstPlayer *parent);
        ~QtGstPlayerPrivate();
        QtGstPlayer * const q_ptr;
	bool playUrl(const QUrl &url);
	bool play();
	bool stop();
	bool pause();
	void seek(const qint64 msec);
private:
        GstElement *m_pipeline;
        GstElement *sink;
	qint64 m_position;
	qint64 m_duration;
        QTimer *m_playTimer;
        bool m_durationSet;
        bool m_canRunout;
        static gboolean bus_callback(GstBus *bus,GstMessage *message, gpointer ptr);
        void updateDuration();
        void emitFinished();
private slots:
        void getTime();
};

#endif // QGSTPLAYER_P_H
