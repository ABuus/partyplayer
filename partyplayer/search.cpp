#include "search.h"

// google developer id of Anders Buus
#define dev_id "AI39si61cUdAWBI2eFoGoRKodF3zuFDyUTTFHyK_X85TDEpjIv6EgeJokdy6UiuG4bq1zri9l4gExE6oEb_xwyAXuxuUgdwdRg"

Search::Search(QObject *parent)
	: QObject(parent)
{
	webPage = new QWebPage(this);
	connect(webPage,SIGNAL(loadFinished(bool)),this,SLOT(queryFinished()));
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
	webPage->mainFrame()->load(url);
}

void Search::queryFinished()
{
	QString html = webPage->mainFrame()->toHtml();
	
	int entryCount = html.count("entry") / 2;

	int offset = 0;
	for(int i = 0; i < entryCount; i++)
	{
		int entryStart = html.indexOf("entry",offset); //		<entry>
		int entryEnd = html.indexOf("entry",entryStart +1);//	</entry>
		
		QString entry = html.mid(entryStart,entryEnd - entryStart);

		int titleStart = entry.indexOf("&lt;title type='text'&gt;") + 25; //	<title>
		int titleEnd = entry.indexOf("&lt;/title&gt;");	//	</title>

		int decStart = entry.indexOf("&lt;media:description type='plain'&gt;") +38; // <media:description type='plain'>
		int decEnd = entry.indexOf("&lt;/media:description&gt;"); // </media:description>

		int urlStart = entry.indexOf("&lt;media:player url='") +22; // <media:player url='
		int urlEnd = entry.indexOf("'/&gt;", urlStart); // '/>

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