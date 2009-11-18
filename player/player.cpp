#include "player.h"

Player::Player(QObject *parent)
	:QObject(parent)
{
	// initialize gstreamer
	gst_init(NULL, NULL);
	// connect elements
	m_pipeline = gst_element_factory_make("playbin", "player");
	m_sink = gst_element_factory_make("autoaudiosink", "player");
	gst_element_link(m_pipeline, m_sink);
	// bus handler
//	m_bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
	m_msgHandler = new MessageHandler(this);
	m_msgHandler->busAddWatch(this);
}

Player::~Player()
{
	gst_element_set_state (GST_ELEMENT (m_pipeline), GST_STATE_NULL);
	gst_object_unref(m_pipeline);
	gst_object_unref(m_sink);
	gst_object_unref(m_bus);
}

void Player::playUrl(const QUrl &url)
{
	if(!url.isValid())
	{
		qDebug() << "invalid url:" << url.toString();
		return;
	}
	gst_element_set_state (GST_ELEMENT (m_pipeline), GST_STATE_NULL);

	// convert QUrl to gchar
	std::string str = std::string(url.toString().toAscii().data());
	const gchar *uri = str.c_str();
	g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);

	gst_element_set_state (GST_ELEMENT (m_pipeline), GST_STATE_PLAYING);
}

void Player::eos()
{
	qDebug() << "End of Stream";
}

void Player::changeState(GstMessage *msg)
{
	qDebug() << "State changed" << GST_STATE(msg);
	gst_object_unref(msg);
}