#include "message.h"

static int wuchi = qRegisterMetaType<Message>();

Message::Message():
	m_message(0)
{}

Message::Message(GstMessage *msg):
	m_message(msg)
{
}

Message::Message(const Message &other)
{
    m_message = other.m_message;
    gst_message_ref(m_message);
}

Message::~Message()
{
	gst_message_unref(m_message);
}

GstMessage *Message::copy()
{
	return m_message;
}