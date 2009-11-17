#ifndef PLAYER_H
#define PLAYER_H

// Qt
#include <QObject>
#include <QUrl>
#include <QDebug>
// gstreamer
#include <gst/gst.h>

#include "player_global.h"

class PLAYER_EXPORT Player : public QObject
{
	Q_OBJECT
public:
	Player(QObject *parent = 0);
	~Player();
public slots:
	void playUrl(const QUrl &url);
private:
	static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer user_data);
	GstElement *m_pipeline;
	GstBus *m_bus;
};

#endif // PLAYER_H
