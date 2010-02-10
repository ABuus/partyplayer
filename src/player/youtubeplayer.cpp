#include "youtubeplayer.h"

YoutubePlayer::YoutubePlayer(QObject *parent)
	: QWebPage(parent),
	m_totalTime(0),
	m_currentTime(0),
	m_state(-1)
{
	/* enable plugins and javascript support */
	settings()->setAttribute(QWebSettings::PluginsEnabled, true);
	settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
	settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
	settings()->clearMemoryCaches();

//	HttpDaemon *server = new HttpDaemon(1919,this);

	connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));
	
	QUrl url = QUrl("http://www.partyplayer.megabyet.net/playerdata/play.html");
	mainFrame()->load(url);
	mainFrame()->evaluateJavaScript("setDebugEnabled(true); null");
}

YoutubePlayer::~YoutubePlayer()
{

}

/* call swf player to play video */
void YoutubePlayer::play()
{
	mainFrame()->evaluateJavaScript("playVideo(); null");
}

/* call swf player to pause video */
void YoutubePlayer::pause()
{
	mainFrame()->evaluateJavaScript("pauseVideo(); null");
}

/* resize the swf player to fit in this without scrollbars */
void YoutubePlayer::resizePlayer(int width, int height)
{
	/* pixel offset to avoid scrollbars */
	int offset = 0;

	QString js = "resizePlayer(%1,%2)";
	setViewportSize(QSize(width,height));
	mainFrame()->evaluateJavaScript(js.arg( width - offset ).arg( height - offset ));
}

/* load the video with vidId, this is not playing the video */
void YoutubePlayer::cueVideoById( QString vidId )
{
	QString js = "cueVideoById('%1');";
	js = js.arg(vidId);
	qDebug() << js;
	mainFrame()->evaluateJavaScript(js);
}

/* load the video with vidId, this is playing the video */
void YoutubePlayer::loadVideoById( QString vidId )
{
	QString js = "loadVideoById('%1');";
	js = js.arg(vidId);
	qDebug() << js;
	mainFrame()->evaluateJavaScript(js);
}

void YoutubePlayer::addJavaScriptObject()
{
	/* make this class an object on the js side */
	mainFrame()->addToJavaScriptWindowObject("clientApp",this);
}

/* this slot is connected in javascript */
void YoutubePlayer::setState(int state)
{
	if(state == m_state)
		return;
	m_state = state;
	emit stateChanged(m_state);
}

void YoutubePlayer::setTotalTime(double time)
{
	qint64 temp = time * 1000;
	if(m_totalTime != temp)
	{
		m_totalTime = temp;
		emit totalTimeChanged(m_totalTime);
	}
}

void YoutubePlayer::setCurrentTime(double time)
{
	qint64 temp = time * 1000;
	if(m_currentTime != temp)
	{
		m_currentTime = temp;
		emit currentTimeChanged(m_currentTime);
	}
}

void YoutubePlayer::handlePlayerError(int errorCode)
{
	qDebug() << "Youtube player error:" << errorCode;
}

void YoutubePlayer::jsDebug(QVariant value)
{
	qDebug() << "debug output from javascript:" << value.toString();
}
