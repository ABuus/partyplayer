#include "search.h"

// google developer id of Anders Buus
#define dev_id "AI39si61cUdAWBI2eFoGoRKodF3zuFDyUTTFHyK_X85TDEpjIv6EgeJokdy6UiuG4bq1zri9l4gExE6oEb_xwyAXuxuUgdwdRg"

Search::Search(QObject *parent)
	: QObject(parent)
{
	netGetter = new QNetworkAccessManager(this);
	connect(netGetter,SIGNAL(finished( QNetworkReply *)),this,SLOT(queryFinished(QNetworkReply *)));
}

Search::~Search()
{
}

void Search::query(QString queryString)
{
	emit newSearch();
	queryString.replace(" ", "+");
	QUrl url( "http://gdata.youtube.com/feeds/api/videos?q=" + queryString + "&format=5&key=" + dev_id);
	Debug << url;
	netGetter->get(QNetworkRequest(url));
//	webPage->mainFrame()->load(url);
}

void Search::queryFinished(QNetworkReply *reply)
{
	QByteArray ba = reply->readAll();
	QString html(ba);
	
	int entryCount = html.count("entry") / 2;

	int offset = 0;
	for(int i = 0; i < entryCount; i++)
	{
		int entryStart = html.indexOf("entry",offset);
		int entryEnd = html.indexOf("entry",entryStart +1);
		
		QString entry = html.mid(entryStart,entryEnd - entryStart);

		int titleStart = entry.indexOf("<title type='text'>") + 19;
		int titleEnd = entry.indexOf("</title>");

		int decStart = entry.indexOf("<media:description type='plain'>") +32;
		int decEnd = entry.indexOf("</media:description>");

		int urlStart = entry.indexOf("<media:player url='") +19;
		int urlEnd = entry.indexOf("'/>", urlStart);

		QString title = entry.mid(titleStart, (titleEnd - titleStart));
		QString dec = entry.mid(decStart, (decEnd - decStart));
		QString url = entry.mid(urlStart, (urlEnd - urlStart));

//		Debug << "\n" << title << "\n" << dec << "\n" << url;

		offset = entryEnd + 1; // offset to next entry
		QStringList item;
		item << title << dec << url;
		emit newItem(item);
	}
}