#include "player.h"

Player::Player(QObject *parent)
	:QObject(parent)
{
	gst_init(NULL, NULL);
	m_pipeline = gst_element_factory_make("playbin", "player");

	m_bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
	gst_bus_add_watch(m_bus, bus_call, NULL);
	
}

Player::~Player()
{
	gst_object_unref(m_pipeline);
	gst_object_unref(m_bus);
}

void Player::playUrl(const QUrl &url)
{
	if(!url.isValid())
	{
		qDebug() << "invalid url:" << url.toString();
		return;
	}
	// convert QUrl to gchar
	std::string str = std::string(url.toString().toAscii().data());
	const gchar *uri = str.c_str();
	g_object_set(G_OBJECT(m_pipeline), "uri", uri, NULL);

	gst_element_set_state (GST_ELEMENT (m_pipeline), GST_STATE_PLAYING);
//	gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
//	gst_object_unref (GST_OBJECT (pipeline));
}

gboolean Player::bus_call(GstBus *bus, GstMessage *msg, gpointer user_data )
{
	qDebug() << msg;
	switch (GST_MESSAGE_TYPE (msg))
	{
		case GST_MESSAGE_EOS:
		{
			g_message ("End-of-stream");
			break;
		}
		case GST_MESSAGE_ERROR:
		{
			gchar *debug;
			GError *err;
			gst_message_parse_error (msg, &err, &debug);
			g_free (debug);
			g_error ("%s", err->message);
			g_error_free (err);
			break;
		}
		default:
			break;
	}
	return true;
}