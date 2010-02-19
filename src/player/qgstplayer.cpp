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

/*
 * --------------- Public Class --------------
 */

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

/*
 * --------------- Private Class --------------
 */

QGstPlayerPrivate::QGstPlayerPrivate()
{
	/* initialise gstreamer */
	gst_init(NULL,NULL);

	/* create gstreamer elements */
	sink = gst_element_factory_make("autoaudiosink", "player");
	pipeline = gst_element_factory_make("playbin", "player");
	gst_element_link(pipeline, sink);

	/* connect bus */
	GstBus *bus;
	bus = gst_element_get_bus(pipeline);
	gst_bus_add_watch(bus,bus_callback,this);
	g_object_unref(bus);
}

QGstPlayerPrivate::~QGstPlayerPrivate()
{
	g_object_unref(pipeline);
	g_object_unref(sink);
	g_main_loop_quit(gLoop);
}

void QGstPlayerPrivate::init()
{
	g_timeout_add(100,syncEventloop,this);
	gLoop = g_main_loop_new(NULL,false);
	g_main_loop_run(gLoop);
}

gboolean QGstPlayerPrivate::bus_callback(GstBus *bus,GstMessage *message, gpointer ptr)
{
	Q_UNUSED(bus);
	QGstPlayerPrivate *thisPtr = static_cast<QGstPlayerPrivate*>(ptr);
	Q_ASSERT(thisPtr);
	switch(GST_MESSAGE_TYPE(message)) {
		case GST_MESSAGE_EOS:
			thisPtr->emitFinished();
			break;
		case GST_MESSAGE_DURATION:
			thisPtr->updateDuration();
			break;
		case GST_MESSAGE_STATE_CHANGED:
			GstState state;
			gst_element_get_state(thisPtr->pipeline,&state,NULL,NULL);
			thisPtr->setState(state);
			break;
	}
	return true;
}

gboolean QGstPlayerPrivate::syncEventloop(gpointer ptr)
{
	Q_UNUSED(ptr);
	QEventLoop qLoop;
	qLoop.processEvents();

	QGstPlayerPrivate *thisPtr = static_cast<QGstPlayerPrivate*>(ptr);
	Q_ASSERT(thisPtr);

	thisPtr->updatePosision();
	
	return true;
}



bool QGstPlayerPrivate::playUrl(const QUrl &url)
{
	gst_element_set_state(pipeline, GST_STATE_NULL);
	QByteArray baUrl = url.toEncoded();
	if(baUrl.isEmpty())
	{
		qDebug() << "playUrl: invalid url" << url;
		return false;
	}
	const gchar *uri = baUrl.constData();
	qDebug() << "QGstreamer play valid url:" << uri;
	g_object_set(G_OBJECT(pipeline), "uri", uri, NULL);
	updateDuration();
	return gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

bool QGstPlayerPrivate::play()
{
	updateDuration();
	return gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

bool QGstPlayerPrivate::pause()
{
	return gst_element_set_state(pipeline, GST_STATE_PAUSED);
}

bool QGstPlayerPrivate::stop()
{
	return gst_element_set_state(pipeline, GST_STATE_NULL);
}

void QGstPlayerPrivate::seek(const qint64 msec)
{
	Q_UNUSED(msec);
	gst_element_seek(pipeline, 1.0, GST_FORMAT_TIME,
		GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, msec * GST_MSECOND,
		GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

bool QGstPlayerPrivate::enqueue(const QUrl &url)
{
	if(url.isValid())
	{
		QByteArray baUrl = url.toEncoded();
		if(baUrl.isEmpty())
		{
			qDebug() << "enqueue invalid url" << url;
			return false;
		}
		const gchar *uri = baUrl.constData();
		g_object_set(G_OBJECT(pipeline), "uri", uri, NULL);
		return true;
	}
	qDebug() << "enqeue failed" << url;
	stop();
	return false;
}

void QGstPlayerPrivate::setState(GstState state)
{
	Q_Q(QGstPlayer);
	if( state == GST_STATE_PLAYING && m_state != 1)
	{
		m_state = 1;
		emit q->stateChanged(m_state);
		return;
	}
	else if( state == GST_STATE_PAUSED && m_state != 2)
	{
		m_state = 2;
		emit q->stateChanged(m_state);
		return;
	}
	else if( ( state == GST_STATE_VOID_PENDING || state == GST_STATE_NULL || state == GST_STATE_READY ) && m_state != 0)
	{
		m_state = 0;
		emit q->stateChanged(m_state);
		return;
	}
}

void QGstPlayerPrivate::emitFinished()
{
	Q_Q(QGstPlayer);
	emit q->finished();
}

void QGstPlayerPrivate::updateDuration()
{
	Q_Q(QGstPlayer);
	GstFormat format = GST_FORMAT_TIME;
	gint64 dur;
	if(gst_element_query_duration(pipeline,&format,&dur))
	{
		durationSet = true;
		m_duration = dur;
		emit q->durationChanged(GST_TIME_AS_MSECONDS(m_duration));
	}
	else
	{
		durationSet = false;
	}
}

void QGstPlayerPrivate::updatePosision()
{
	Q_Q(QGstPlayer);
	if(!durationSet)
	{
		updateDuration();
	}
	GstFormat format = GST_FORMAT_TIME;
	gint64 posision;
	gst_element_query_position(pipeline,&format,&posision);
	if(m_posision != posision)
	{	
		m_posision = posision;
		emit q->timeChanged( GST_TIME_AS_MSECONDS(m_posision));
	}
}
