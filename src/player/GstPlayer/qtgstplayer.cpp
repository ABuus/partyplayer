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

#include "qtgstplayer.h"
#include "qtgstplayer_p.h"
#include <QDebug>

/*
 * --------------- Public Class --------------
 */

QtGstPlayer::QtGstPlayer(QObject *parent)
	: QObject(parent),
        d_ptr(new QtGstPlayerPrivate(this))
{}

QtGstPlayer::QtGstPlayer(QtGstPlayerPrivate &dd, QObject *parent)
	: QObject(parent),
	d_ptr(&dd)
{}

QtGstPlayer::~QtGstPlayer()
{}

bool QtGstPlayer::playUrl(const QUrl &url)
{
        Q_D(QtGstPlayer);
	return d->playUrl(url);
}

bool QtGstPlayer::play()
{
        Q_D(QtGstPlayer);
	return d->play();
}
bool QtGstPlayer::pause()
{
        Q_D(QtGstPlayer);
	return d->pause();
}

bool QtGstPlayer::stop()
{
        Q_D(QtGstPlayer);
	return d->stop();
}

void QtGstPlayer::seek(const qint64 msec)
{
        Q_D(QtGstPlayer);
	d->seek(msec);
}

/*
 * --------------- Private Class --------------
 */

QtGstPlayerPrivate::QtGstPlayerPrivate(QtGstPlayer *parent)
    :q_ptr(parent),
    m_playTimer(new QTimer(this)),
    m_durationSet(false),
    m_canRunout(true)
{
	/* initialise gstreamer */
	GError **err = NULL;
	if(!gst_init_check(NULL,NULL,err))
	{
		qDebug() << err;
	}
	g_object_unref(err);

	/* create gstreamer elements */
	sink = gst_element_factory_make("autoaudiosink", "player");
        m_pipeline = gst_element_factory_make("playbin", "player");
        gst_element_link(m_pipeline, sink);

        /* connect gstreamer bus */
	GstBus *bus;
        bus = gst_element_get_bus(m_pipeline);
	gst_bus_add_watch(bus,bus_callback,this);
	g_object_unref(bus);

        m_playTimer->setInterval(PLAYER_TIMERINTERVAL);
        QObject::connect(m_playTimer,SIGNAL(timeout()),this,SLOT(getTime()));
}

QtGstPlayerPrivate::~QtGstPlayerPrivate()
{
        g_object_unref(m_pipeline);
	g_object_unref(sink);
}

gboolean QtGstPlayerPrivate::bus_callback(GstBus *bus,GstMessage *message, gpointer ptr)
{
	Q_UNUSED(bus);
        QtGstPlayerPrivate *thisPtr = static_cast<QtGstPlayerPrivate*>(ptr);
	Q_ASSERT(thisPtr);
	switch(GST_MESSAGE_TYPE(message)) {
		case GST_MESSAGE_EOS:
			thisPtr->emitFinished();
			break;
		case GST_MESSAGE_DURATION:
			thisPtr->updateDuration();
			break;
/*		case GST_MESSAGE_STATE_CHANGED:
			GstState state;
                        gst_element_get_state(thisPtr->m_pipeline,&state,NULL,GST_CLOCK_TIME_NONE);
			thisPtr->setState(state);
			break;
*/
		default:
			break;
	}
	return true;
}

void QtGstPlayerPrivate::emitFinished()
{
    Q_Q(QtGstPlayer);
    emit q->finished();
}


bool QtGstPlayerPrivate::playUrl(const QUrl &url)
{
        gst_element_set_state(m_pipeline, GST_STATE_NULL);
	QByteArray baUrl = url.toEncoded();
	if(baUrl.isEmpty())
	{
		qDebug() << "playUrl: invalid url" << url;
		return false;
	}
	const gchar *uri = baUrl.constData();
	qDebug() << "QGstreamer play valid url:" << uri;
        g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);
//	updateDuration();
        return gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}

bool QtGstPlayerPrivate::play()
{
//	updateDuration();
        return gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}

bool QtGstPlayerPrivate::pause()
{
        return gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
}

bool QtGstPlayerPrivate::stop()
{
        return gst_element_set_state(m_pipeline, GST_STATE_NULL);
}

void QtGstPlayerPrivate::seek(const qint64 msec)
{
	Q_UNUSED(msec);
        gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME,
		GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, msec * GST_MSECOND,
		GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

void QtGstPlayerPrivate::getTime()
{
        Q_Q(QtGstPlayer);
        if(!m_durationSet)
        {
//                getTotalTime();
                return;
        }
        GstFormat fmt = GST_FORMAT_TIME;
        gint64 pos;
        if( gst_element_query_position(m_pipeline, &fmt, &pos))
        {
                emit q->positionChanged(GST_TIME_AS_MSECONDS(pos));
                // check if we are running out
                if( ( GST_TIME_AS_MSECONDS( m_duration ) - GST_TIME_AS_MSECONDS( pos ) ) < 1000 )
                {
    //			Debug << "inside run out:" << (GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ));
                        if(m_canRunout)
                        {
                                m_canRunout = false;
                                qDebug() << "running out with:" << ( GST_TIME_AS_MSECONDS( m_duration ) - GST_TIME_AS_MSECONDS( pos ) );
                                emit q->runningOut();
                        }
                }
        }
        else
        {
                qDebug() << "Could not get posision";
        }
}


void QtGstPlayerPrivate::updateDuration()
{
        Q_Q(QtGstPlayer);
	GstFormat format = GST_FORMAT_TIME;
	gint64 dur;
        if(gst_element_query_duration(m_pipeline,&format,&dur))
	{
                m_durationSet = true;
                m_duration = dur;
		emit q->durationChanged(GST_TIME_AS_MSECONDS(m_duration));
	}
	else
	{
                m_durationSet = false;
	}
}

/*
void QtGstPlayerPrivate::updatePosition()
{
        Q_Q(QtGstPlayer);
        if(!q->d_ptr->durationSet)
	{
		updateDuration();
	}
	GstFormat format = GST_FORMAT_TIME;
	gint64 position;
	gst_element_query_position(pipeline,&format,&position);
        qDebug() << "QtGstPlayer positionChanged emitted: " << m_position;
	if(m_position != position)
	{	
		m_position = position;
		emit q->positionChanged( GST_TIME_AS_MSECONDS(m_position));
	}
}
*/
