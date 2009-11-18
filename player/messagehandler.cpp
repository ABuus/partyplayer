#include "messagehandler.h"
#include "player.h"

MessageHandler::MessageHandler(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<Message>("Message");
}

MessageHandler::~MessageHandler()
{

}

void MessageHandler::busAddWatch(Player *player)
{
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(player->pipeline()));
	gst_bus_add_watch(bus,bus_call,player);
	gst_object_unref(bus);
}

gboolean MessageHandler::bus_call(GstBus * /* bus */, GstMessage *msg, gpointer ptr )
{
	Player *player = static_cast<Player *>(ptr);
	
	switch (GST_MESSAGE_TYPE (msg))
	{
		case GST_MESSAGE_EOS:
		{
			QMetaObject::invokeMethod(player->messageHandler(), "eos", Qt::QueuedConnection);
		}
		case GST_MESSAGE_STATE_CHANGED:
		{
			QMetaObject::invokeMethod(player->messageHandler(), "changeState", Qt::QueuedConnection, Q_ARG(Message, msg) );
			//player->changeState(msg);
		}
	}
	
	return true;
}