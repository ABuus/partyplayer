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

	QDomDocument doc;
	doc.setContent(html);
	QDomElement docElem = doc.documentElement();
	QDomNodeList nodeList = docElem.elementsByTagName("entry");

	for(int i = 0; i < nodeList.count(); i++)
	{
		QDomElement entry = nodeList.at(i).toElement();
		QString title = entry.elementsByTagName("title").at(0).firstChild().nodeValue();
		QString description = entry.elementsByTagName("media:description").at(0).firstChild().nodeValue();
		
		QString id = entry.elementsByTagName("id").at(0).firstChild().nodeValue();
		id.remove("http://gdata.youtube.com/feeds/api/videos/");

		// mg is a media:group see http://code.google.com/apis/youtube/2.0/reference.html#youtube_data_api_tag_media:group
		QDomElement mg = entry.elementsByTagName("media:group").at(0).toElement();
		QString duration = mg.elementsByTagName("yt:duration").at(0).toElement().attributeNode("seconds").value();
		Debug << title << description << id << duration;

		
		// http://code.google.com/apis/youtube/player_parameters.html
		// there are more parameters to be supported eg hd
		QString url("http://www.youtube.com/v/");
		url.append(id);
		
		url.append("?autoplay=1"); // start playing on load
		url.append("&iv_load_policy=3"); 
		url.append("&showinfo=0&");
		url.append("enablejsapi=1"); // enables JavaScript control layer
		
		
		
		QStringList item;
		item << title << description << url << duration;
		emit newItem(item);
	}
/*
	for(int i = 0; i < entryCount; i++)
	{
		
		Debug << "entry count" << 

		int entryStart = html.indexOf("entry",offset);
		int entryEnd = html.indexOf("entry",entryStart +1);
		
		QString entry = html.mid(entryStart,entryEnd - entryStart);

		int titleStart = entry.indexOf("<title type='text'>") + 19;
		int titleEnd = entry.indexOf("</title>");

		int decStart = entry.indexOf("<media:description type='plain'>") +32;
		int decEnd = entry.indexOf("</media:description>");
		
		int idStart = entry.indexOf("<yt:videoid>") +11;
		int idEnd = entry.indexOf("</yt:videoid>", idStart);

		int durationStart = entry.indexOf("<yt:duration seconds='") +22;
		int durationEnd = entry.indexOf("'/>", durationStart);

		QString title = entry.mid(titleStart, (titleEnd - titleStart));
		QString dec = entry.mid(decStart, (decEnd - decStart));
		QString id = entry.mid(idStart, (idEnd - idStart));
		QString duration = entry.mid(durationStart, (durationEnd - durationStart));

		// http://code.google.com/apis/youtube/player_parameters.html
		// there are more parameters to be supported eg hd
		QString url("http://www.youtube.com/v/");
		url.append(id);
		url.append("?autoplay=1&iv_load_policy=3&showinfo=0");

		title.replace("&amp;","&");
		dec.replace("&amp;","&");
		
//		Debug << "\nTitle:" << title << "\ndescirption:" << dec << "\nurl:" << url << "\nduration:" << duration;

		offset = entryEnd + 1; // offset to next entry
		QStringList item;
		item << title << dec << url << duration;
		emit newItem(item);
	}
	*/
}

