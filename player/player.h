#ifndef PLAYER_H
#define PLAYER_H

// Qt
#include <QObject>
#include <QUrl>
#include <QDebug>
// gstreamer
#include <gst/gst.h>

#include "player_global.h"
#include "messagehandler.h"

class PLAYER_EXPORT Player : public QObject
{
	Q_OBJECT
public:
	Player(QObject *parent = 0);
	~Player();
	GstElement *pipeline() { return m_pipeline; };
	MessageHandler *messageHandler() { return m_msgHandler; };
public slots:
	void playUrl(const QUrl &url);
private:
//	static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer user_data);
	MessageHandler *m_msgHandler;
	GstElement *m_pipeline;
	GstElement *m_sink;
	GstBus *m_bus;
	void eos();
	void changeState(GstMessage *msg);
};

#endif // PLAYER_H
