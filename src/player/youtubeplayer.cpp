#include "youtubeplayer.h"

YoutubePlayer::YoutubePlayer(QObject *parent)
	: QWebPage(parent),
	m_totalTime(0),
	m_currentTime(0),
	m_state(-1),
	m_playerQuality("default")
{
	/* enable plugins and javascript support */
	settings()->setAttribute(QWebSettings::PluginsEnabled, true);
	settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
	settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
	settings()->setMaximumPagesInCache(0);
	settings()->setObjectCacheCapacities(0,0,0);
	settings()->clearMemoryCaches();

	connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));
	
	QUrl url = QUrl("http://www.partyplayer.megabyet.net/playerdata/play.html");
	mainFrame()->load(url);
	mainFrame()->evaluateJavaScript("setDebugEnabled(true); null");
}

YoutubePlayer::~YoutubePlayer()
{

}

bool YoutubePlayer::playUrl(const QUrl url)
{
	QString urlStr = url.toString();
	if(!urlStr.startsWith("http://www.youtube.com/watch?v="))
		return false;
	urlStr.remove("http://www.youtube.com/watch?v=");
	loadVideoById(urlStr);
	return true;
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

void YoutubePlayer::seek(int msec)
{
	// true is allow seek ahead youtube api
	QString js = "seekTo(%1,true); null"; 
	mainFrame()->evaluateJavaScript(js.arg(msec / 1000));
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
	Debug << js;
	mainFrame()->evaluateJavaScript(js);
}

/* load the video with vidId, this is playing the video */
void YoutubePlayer::loadVideoById( QString vidId )
{
	if(vidId.startsWith("http://www.youtube.com/watch?v="))
		vidId.remove("http://www.youtube.com/watch?v=");
	QString js = "loadVideoById('%1',0,'%2');";
	js = js.arg(vidId);
	js = js.arg(m_playerQuality);
	Debug << js;
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
	if(m_state == 0)
		emit finished();
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
	Q_UNUSED(value);
//	qDebug() << "debug output from javascript:" << value.toString();
}

void YoutubePlayer::setPlayQuality(int playerQualety)
{
	switch(playerQualety) {
		case YoutubePlayer::Low:
			m_playerQuality = "small";
			break;
		case YoutubePlayer::Medium:
			m_playerQuality = "medium";
			break;
		case YoutubePlayer::High:
			m_playerQuality = "large";
			break;
		case YoutubePlayer::Hd720:
			m_playerQuality = "hd720";
			break;
		case YoutubePlayer::Standard:
			m_playerQuality = "default";
			break;
	};
	QString js = "setPlaybackQuality('%1');";
	mainFrame()->evaluateJavaScript(js.arg(m_playerQuality));
}