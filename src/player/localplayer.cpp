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

#include "localplayer.h"

LocalPlayer::LocalPlayer(QObject *parent)
	:QObject(parent),
	m_pipeline(0),
	m_canRunOut(true),
	m_playTimer(this),
	m_totaltime(0),
	m_totalTimeSet(false),
	m_state(0)
{
	// initialize gstreamer
	GError *err;
	if(!gst_init_check(NULL, NULL,&err))
	{
		g_object_unref(err);
		Debug << err;
	}
	
	// create gst elemente
	m_sink = gst_element_factory_make("autoaudiosink", "sink");
	m_pipeline = gst_element_factory_make("playbin","pipeline");
	// link elements
	gst_element_link(m_pipeline, m_sink);
//	g_timeout_add (TIMER_INTERVAL, NULL, m_pipeline);
	
	// start play timer
	m_playTimer.setInterval(TIMER_INTERVAL);
	connect(&m_playTimer,SIGNAL(timeout()), this, SLOT( getTime()));
}

LocalPlayer::~LocalPlayer()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_object_unref(m_pipeline);
	gst_object_unref(m_sink);
	gst_deinit();
}

void LocalPlayer::playUrl(const QUrl &url)
{
	if(!url.isValid())
	{
		Debug << "invalid url:" << url.toString();
		m_playTimer.stop();
		return;
	}

	gst_element_set_state(m_pipeline, GST_STATE_NULL);

	QByteArray baUrl = url.toEncoded();
	if(baUrl.isEmpty())
		Debug << "empty baUrl";
	const gchar *uri = baUrl.constData();
	g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);
	gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
#ifndef Q_WS_WIN
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
	gst_bus_add_watch(bus,cb_gst_bus,this);
	g_object_unref(bus);
#endif

	// get time
	getTotalTime();
	m_playTimer.start();
	checkState();
}

void LocalPlayer::getTime()
{
	if(!m_totalTimeSet)
	{
		getTotalTime();
		return;
	}
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 pos;
	if( gst_element_query_position(m_pipeline, &fmt, &pos))
	{
		emit timeChanged( GST_TIME_AS_MSECONDS( pos ) );
		// check if we are running out
		if( ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) ) < 1000 )
		{
//			Debug << "inside run out:" << (GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ));
			if(m_canRunOut)
			{
				m_canRunOut = false;
				Debug << "running out with:" << ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) );
				emit runningOut();
			}		
#ifdef Q_WS_WIN
			// on windows we use this to get total time on linux bus_callback Q: why? A: The usual, gstreamer is fucked without GTK mainloop.
			// check if we are at end :: 50 msec gstreamer can be at EOS with 18 msec to end ??
			if( ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) ) <= 50 ) 
			{
				Debug << "EOS";
				m_playTimer.stop();
				getTotalTime();
				checkState();
				emit finished();
			}
#endif
		}
		else
		{
			Debug << "current pos" << GST_TIME_AS_MSECONDS(pos) << "with total time" << GST_TIME_AS_MSECONDS(m_totaltime);
		}
	}
	else
	{
		Debug << "Could not get posision";
	}
}

void LocalPlayer::getTotalTime()
{

	GstFormat fmt = GST_FORMAT_TIME;
	gint64 tot;
	if(!gst_element_query_duration(m_pipeline, &fmt, &tot))
	{

		/* gstreamer did not deliver the duration we will try again on next timout event */
		m_totalTimeSet = false;
		return;
	}
	else 
	{
		/* we got the total time */
		m_totalTimeSet = true;
		/* if we quered gstreamer for the duration we are playing, so we can run out */
		m_canRunOut = true;
		if(m_totaltime == tot)
			return;
		m_totaltime = tot;
		emit totalTimeChanged( GST_TIME_AS_MSECONDS( tot ) );
	}
}

void LocalPlayer::seek(int time)
{
	gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME,
		GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, time * GST_MSECOND,
		GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

void LocalPlayer::play()
{
	gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
	m_playTimer.start();
	checkState();
}

void LocalPlayer::stop()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	m_playTimer.stop();
	checkState();
}

void LocalPlayer::pause()
{
	gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
	m_playTimer.stop();
	checkState();
}

void LocalPlayer::checkState()
{
	GstState state;
	gst_element_get_state(m_pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
	if( state == GST_STATE_PLAYING && m_state != 1)
	{
		m_state = 1;
		emit stateChanged(m_state);
		return;
	}
	else if( state == GST_STATE_PAUSED && m_state != 2)
	{
		m_state = 2;
		emit stateChanged(m_state);
		return;
	}
	else if( ( state == GST_STATE_VOID_PENDING || state == GST_STATE_NULL || state == GST_STATE_READY ) && m_state != 0)
	{
		m_state = 0;
		emit stateChanged(m_state);
		return;
	}
}

#ifndef Q_WS_WIN

gboolean LocalPlayer::cb_gst_bus(GstBus* bus,GstMessage* message,gpointer data)
{
	Q_UNUSED(bus);
	LocalPlayer *m_this = static_cast<LocalPlayer*>(data);
	switch(GST_MESSAGE_TYPE(message)) {
		case GST_MESSAGE_EOS:
			emit m_this->finished();
			break;
		case GST_MESSAGE_DURATION: 
			QMetaObject::invokeMethod(m_this,"getTotalTime",Qt::QueuedConnection);
			break;
		default:
			break;
	}
	return true;
}

#endif