#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
// gstreamer
#include <gst/gst.h>
#include <gst/interfaces/propertyprobe.h>
// phonon helper
#include "message.h"
#include "player_global.h"

class Player;

class PLAYER_EXPORT MessageHandler : public QObject
{
	Q_OBJECT

public:
	MessageHandler(QObject *parent = 0);
	~MessageHandler();
	void busAddWatch(Player *player);
private:
	static gboolean bus_call( GstBus *bus, GstMessage *msg, gpointer ptr);
};

#endif // MESSAGEHANDLER_H
