/*
	copy of phonon to use gstreamer on win
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore/QMetaType>
#include <gst/gst.h>

class Message
{
	
public:
	Message();
	Message(GstMessage *msg);
	~Message();
	Message(const Message &other);

	GstMessage *copy();
private:
	GstMessage *m_message;
	
};

Q_DECLARE_METATYPE(Message)

#endif // MESSAGE_H
