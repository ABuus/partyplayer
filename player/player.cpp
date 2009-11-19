#include "player.h"

Player::Player(QObject *parent)
	:QObject(parent),
	m_playTimer(this),
	m_totaltime(0)
{
	// initialize gstreamer
	gst_init(NULL, NULL);
	// connect elements
	m_pipeline = gst_element_factory_make("playbin", "player");
	m_sink = gst_element_factory_make("autoaudiosink", "player");
	gst_element_link(GST_ELEMENT( m_pipeline), m_sink);
	g_timeout_add (200, 0, m_pipeline);
	m_playTimer.setInterval(200);
	connect(&m_playTimer,SIGNAL(timeout()), this, SLOT( getTime()));
}

Player::~Player()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
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

	// convert QUrl to gchar
	std::string str = std::string(url.toString().toAscii().data());
	const gchar *uri = str.c_str();
	g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);
	
	gst_element_set_state (m_pipeline, GST_STATE_PLAYING);

	// get time
	m_playTimer.start();
	getTotalTime();
}

void Player::getTime()
{
	
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 pos;
	if( gst_element_query_position(m_pipeline, &fmt, &pos))
	{
		emit timeChanged( GST_TIME_AS_MSECONDS( pos ) );
	}
}

void Player::getTotalTime()
{
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 tot;
	while(! gst_element_query_duration(m_pipeline, &fmt, &tot))
	{}
	m_totaltime = tot;
	emit totalTimeChanged( GST_TIME_AS_MSECONDS( tot ) );
}

void Player::seek(int time)
{
	// gint64 m_time = time;
	gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME,
		GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, time * GST_MSECOND,
		GST_SEEK_TYPE_NONE,GST_CLOCK_TIME_NONE);
}