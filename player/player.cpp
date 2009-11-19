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
		return;
	}
	gst_element_set_state (m_pipeline, GST_STATE_NULL);
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
		if( ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) ) < 1000 )
		{
			if(m_canRunOut)
			{
				m_canRunOut = false;
				qDebug() << "running out with:" << ( GST_TIME_AS_MSECONDS( m_totaltime ) - GST_TIME_AS_MSECONDS( pos ) );
				emit runningOut();
			}
		}
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
		m_newPipeline = createPipeline();
		std::string str = std::string(url.toString().toAscii().data());
		const gchar * uri = str.c_str();
		g_object_set(G_OBJECT(m_newPipeline), "uri", uri, NULL);
		return true;
	}
	return false;
}

GstElement * Player::createPipeline()
{
	GstElement *pipeline = gst_element_factory_make("playbin", "player");
	gst_element_link(GST_ELEMENT( m_pipeline), m_sink);
	g_timeout_add (200, 0, pipeline);
	return pipeline;
}