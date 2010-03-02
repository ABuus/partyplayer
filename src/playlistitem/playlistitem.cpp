#include "../mainapp/application.h"
#include "playlistitem.h"

namespace PlaylistItem {

PlaylistItem::PlaylistItem(const QString &location,QObject *parent)
	:QObject(parent)
{
	m_url = location.startsWith(FILE_MARCO) ? QString(location).remove(0,sizeof(FILE_MARCO)-1) : location;
	init();
}

PlaylistItem::PlaylistItem(const QUrl &url,QObject *parent)
	:QObject(parent)
{
	m_url = url.toString();
	m_url = m_url.startsWith(FILE_MARCO) ? m_url.remove(0,sizeof(FILE_MARCO)-1) : m_url;
	init();
}

PlaylistItem::~PlaylistItem()
{
}

void PlaylistItem::init()
{
	setValid(false);
	Application *app = (Application*)static_cast<QCoreApplication*>(QApplication::instance());
	Q_ASSERT(app);
	m_loadInfoOnline = app->loadOnlineInfo;
}

QString PlaylistItem::url() const
{
	return m_url;
}

QString PlaylistItem::lengthAsPrittyPrint(uint length)
{
	int min = length / 60;
	int sec = length % 60;
	const QString retval = sec < 10 ? QString("%1:0%2").arg(min).arg(sec) : QString("%1:%2").arg(min).arg(sec);
	return retval;
}

}; // namespace PlaylistItem