#ifndef YOUTUBEITEM_H
#define YOUTUBEITEM_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDomDocument>
#include <QDomElement>
#include "playlistitem_global.h"
#include "playlistitem.h"
#include "../debug.h"

// google developer id of Anders Buus
#define dev_id "AI39si61cUdAWBI2eFoGoRKodF3zuFDyUTTFHyK_X85TDEpjIv6EgeJokdy6UiuG4bq1zri9l4gExE6oEb_xwyAXuxuUgdwdRg"

namespace PlaylistItem {

class PLAYLISTITEM_EXPORT YoutubeItem : public PlaylistItem
{
	Q_OBJECT

public:
	YoutubeItem(const QString &url,QObject *parent = 0);
	YoutubeItem(const QUrl &url,QObject *parent = 0);
	~YoutubeItem();
	QString artist() const { return m_artist; };
	QString comment() const { return m_comments; };
	uint length() const { return m_length; };
	QPixmap image() const { return m_image; };
	bool isAsync() const { return true; };
	void fetchData();
private:
	QString m_artist;
	QString m_comments;
	uint m_length;
	QString m_thumbnailUrl;
	QPixmap m_image;
	void init();
	enum ReplyType { NoType, XmlType, ThumbnailType };
	int m_replyType;
	QNetworkAccessManager *manager;
	void phraseXmlData(const QByteArray &xmlData);
	void saveToImage(const QByteArray &data);
	int type() const { return PlaylistItem::Youtube; };
private slots:
	void requestFinished(QNetworkReply *reply);	
};

}; // namespace PlaylistItem

#endif // YOUTUBEITEM_H
