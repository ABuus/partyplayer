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
	m_newPipeline(0),
	m_canRunOut(true),
	m_playTimer(this),
	m_totaltime(0),
	m_totalTimeSet(false),
	m_state(0)
{
	// initialize gstreamer
	GError *err;
	if(!gst_init_check(NULL, NULL,&err))
		Debug << err;
	
	// create gst elemente
	m_sink = gst_element_factory_make("autoaudiosink", "player");
	m_pipeline = createPipeline();
	// connect elements
	gst_element_link(GST_ELEMENT( m_pipeline), m_sink);
	g_timeout_add (TIMER_INTERVAL, NULL, m_pipeline);
	
	// start play timer
	m_playTimer.setInterval(TIMER_INTERVAL);
	connect(&m_playTimer,SIGNAL(timeout()), this, SLOT( getTime()));
}

LocalPlayer::~LocalPlayer()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_object_unref(m_newPipeline);
	gst_object_unref(m_pipeline);
	gst_object_unref(m_sink);
}

void LocalPlayer::playUrl(const QUrl &url)
{
	/* convert url to string that windows accepts as network share */
	QString str_file = url.toLocalFile();
	str_file.prepend("file:///");

	if(!url.isValid())
	{
		Debug << "invalid url:" << str_file;
		m_playTimer.stop();
		return;
	}

	qDebug() << __FILE__ << __LINE__ << "localplayer playing:" << str_file;

	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	std::string str = std::string(str_file.toAscii().data());
	const gchar * uri = str.c_str();
	g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);
	gst_element_set_state(m_pipeline, GST_STATE_PLAYING);

	// get time
	getTotalTime();
	m_playTimer.start();
	checkState();
}

void LocalPlayer::getTime()
{
	if(!m_totalTimeSet)
		getTotalTime();
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 pos;
	if( gst_element_query_position(m_pipeline, &fmt, &pos))
	{
		emit timeChanged( GST_TIME_AS_MSECONDS( pos ) );
		// check if we are running out
		if( ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) ) < 1000 )
		{
			if(m_canRunOut)
			{
				m_canRunOut = false;
				Debug << "running out with:" << ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) );
				emit runningOut();
			}
			// check if we are at end
			if( ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) ) <= 0 )
			{
				Debug << "play next";
				m_playTimer.stop();
				if(!gst_element_set_state(m_newPipeline, GST_STATE_PLAYING))
				{
					Debug << "Failed to change state on new pipeline";
					checkState();
					return;
				}
				if(!gst_element_set_state(m_pipeline, GST_STATE_NULL))
				{
					Debug << "Failed to change state on old pipeline";
					checkState();
					return;
				}
				gst_object_unref(m_pipeline);
				m_pipeline = m_newPipeline;
				getTotalTime();
				m_playTimer.start();
				m_canRunOut = true;
				checkState();
			}
		}
		
	}
}

void LocalPlayer::getTotalTime()
{
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 tot;
	if(!gst_element_query_duration(m_pipeline, &fmt, &tot))
	{
		m_totalTimeSet = false;
		return;
	}
	else {
		m_totalTimeSet = true;
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

bool LocalPlayer::enqueue(const QUrl &url)
{
	Debug << "Enqueue next";
	if(url.isValid())
	{
		Debug << "valid url:" << url;
		m_newPipeline = createPipeline();
		std::string str = std::string(url.toString().toAscii().data());
		const gchar * uri = str.c_str();
		g_object_set(G_OBJECT(m_newPipeline), "uri", uri, NULL);
		return true;
	}
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	emit timeChanged(0);
	m_playTimer.stop();
	checkState();
	return false;
}

GstElement * LocalPlayer::createPipeline()
{
	GstElement *pipeline = gst_element_factory_make("playbin", "player");
	gst_element_link(GST_ELEMENT( pipeline), m_sink);
	g_timeout_add (TIMER_INTERVAL, 0, pipeline);
	return pipeline;
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
