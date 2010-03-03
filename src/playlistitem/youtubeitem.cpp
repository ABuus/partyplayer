#include "youtubeitem.h"


namespace PlaylistItem {

YoutubeItem::YoutubeItem(const QString &url,QObject *parent)
	: PlaylistItem(url,parent)
{
}

YoutubeItem::YoutubeItem(const QUrl &url,QObject *parent)
	: PlaylistItem(url,parent)
{
}

YoutubeItem::~YoutubeItem()
{
	delete manager;
}

void YoutubeItem::fetchData()
{
	QUrl data_url = QString(url()).replace("http://www.youtube.com/watch?v=","http://gdata.youtube.com/feeds/api/videos/");
	data_url.addQueryItem("key",dev_id);
	manager = new QNetworkAccessManager(this);
	connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
	m_replyType = XmlType;
	manager->get(QNetworkRequest(data_url));
}

void YoutubeItem::requestFinished(QNetworkReply *reply)
{
	switch(m_replyType) {
		case XmlType:
			phraseXmlData(reply->readAll());
			break;
		case ThumbnailType:
			saveToImage(reply->readAll());
			break;
		default:
			break;
	}
}

void YoutubeItem::phraseXmlData(const QByteArray &xmlData)
{
	QDomDocument doc;
	doc.setContent(xmlData);
	QDomElement entry = doc.documentElement();
	QDomElement mediaGroup = entry.firstChildElement("media:group");
	
	m_artist = entry.firstChildElement("title").text();
	m_comments = mediaGroup.firstChildElement("media:description").text();
	m_length = mediaGroup.firstChildElement("yt:duration").attribute("seconds").toInt();
	m_thumbnailUrl = mediaGroup.firstChildElement("media:thumbnail").attribute("url");

	Debug << m_thumbnailUrl;

	m_replyType = ThumbnailType;
	manager->get(QNetworkRequest(m_thumbnailUrl));
}

void YoutubeItem::saveToImage(const QByteArray &data)
{
	m_image.loadFromData(data.fromBase64(data.toBase64()));
	setValid(true);
	emit dataRecived();
}

};