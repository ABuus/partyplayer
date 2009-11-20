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

#include "player.h"

Player::Player(QObject *parent)
	:QObject(parent),
	m_playTimer(this),
	m_totaltime(0),
	m_pipeline(0),
	m_newPipeline(0),
	m_canRunOut(true)
{
	// initialize gstreamer
	gst_init(NULL, NULL);
	// connect elements
//	m_pipeline = gst_element_factory_make("playbin", "player");
	m_pipeline = createPipeline();
	m_sink = gst_element_factory_make("autoaudiosink", "player");
	gst_element_link(GST_ELEMENT( m_pipeline), m_sink);
	g_timeout_add (200, 0, m_pipeline);
	m_playTimer.setInterval(200);
	connect(&m_playTimer,SIGNAL(timeout()), this, SLOT( getTime()));
}

Player::~Player()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_object_unref(m_newPipeline);
	gst_object_unref(m_pipeline);
	gst_object_unref(m_sink);
}

void Player::playUrl(const QUrl &url)
{
	if(!url.isValid())
	{
		qDebug() << "invalid url:" << url.toString();
		m_playTimer.stop();
		return;
	}
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	std::string str = std::string(url.toString().toAscii().data());
	const gchar * uri = str.c_str();
	g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);
	
	gst_element_set_state (m_pipeline, GST_STATE_PLAYING);

	// get time
	getTotalTime();
	m_playTimer.start();
}

void Player::getTime()
{
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
				qDebug() << "running out with:" << ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) );
				emit runningOut();
			}
		}
		// check if we are at end
		if( ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) ) == 0 )
		{
			qDebug() << "### play next ###";
			m_playTimer.stop();
			gst_element_set_state(m_newPipeline, GST_STATE_PLAYING);
			gst_element_set_state(m_pipeline, GST_STATE_NULL);
			gst_object_unref(m_pipeline);
			m_pipeline = m_newPipeline;
			getTotalTime();
			m_playTimer.start();
			m_canRunOut = true;
		}
	}
}

void Player::getTotalTime()
{
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 tot;
	while( !gst_element_query_duration(m_pipeline, &fmt, &tot))
	{}
	if(m_totaltime == tot)
		return;
	m_totaltime = tot;
	emit totalTimeChanged( GST_TIME_AS_MSECONDS( tot ) );
}

void Player::seek(int time)
{
	gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME,
		GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, time * GST_MSECOND,
		GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

void Player::play()
{
	gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
	m_playTimer.start();
}

void Player::stop()
{
	gst_element_set_state (m_pipeline, GST_STATE_NULL);
	m_playTimer.stop();
}

void Player::pause()
{
	gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
	m_playTimer.stop();
}

bool Player::enqueue(const QUrl &url)
{
	qDebug() << "### enqueue next ###";
	if(url.isValid())
	{
		qDebug() << "valid url:" << url;
		m_newPipeline = createPipeline();
		std::string str = std::string(url.toString().toAscii().data());
		const gchar * uri = str.c_str();
		g_object_set(G_OBJECT(m_newPipeline), "uri", uri, NULL);
		return true;
	}
	emit timeChanged(0);
	m_playTimer.stop();
	return false;
}

GstElement * Player::createPipeline()
{
	GstElement *pipeline = gst_element_factory_make("playbin", "player");
	gst_element_link(GST_ELEMENT( m_pipeline), m_sink);
	g_timeout_add (200, 0, pipeline);
	return pipeline;
}