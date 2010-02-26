#include "youtubeplayer.h"

/**
 * Contructs a new instance of YoutubePlayer
 * \sa YoutubeViewer
 * \todo add setJavascriptPlayerPath(const QUrl & url)
 * and remove fixed link here.
 * \todo add setJavascriptDebugEnabled(bool enabled = false).
 */

YoutubePlayer::YoutubePlayer(QObject *parent)
	: QWebPage(parent),
	m_totalTime(0),
	m_currentTime(0),
	m_state(-1),
	m_playerQuality("default")
{
	QWebSettings::clearMemoryCaches();
	QWebSettings::setMaximumPagesInCache(0);
	QWebSettings::setObjectCacheCapacities(0,0,0);
	/* enable plugins and javascript support */
	settings()->setAttribute(QWebSettings::PluginsEnabled, true);
	settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
	settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
#ifdef WEB_DEBUG
	settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
#endif
	settings()->setMaximumPagesInCache(0);
	settings()->setObjectCacheCapacities(0,0,0);
	settings()->clearMemoryCaches();

	connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),this, SLOT(addJavaScriptObject()));
	
	QUrl url = QUrl("http://www.partyplayer.megabyet.net/playerdata/play_v2.html");
	mainFrame()->load(url);
}

YoutubePlayer::~YoutubePlayer()
{

}

/**
 * Plays \a url.
 * 
 * Accepted \a url http://www.youtube.com/watch?v= + <yt:videoid>\n
 * This is usefull for drag & drop operations from a external browser.
 * \sa cueVideoById(), loadVideoById()
 */

bool YoutubePlayer::playUrl(const QUrl url)
{
	QString urlStr = url.toString();
	if(!urlStr.startsWith("http://www.youtube.com/watch?v="))
		return false;
	urlStr.remove("http://www.youtube.com/watch?v=");
	loadVideoById(urlStr);
	return true;
}

/** 
 * Start playing a video.
 * \sa cueVideoById()
 */

void YoutubePlayer::play()
{
	mainFrame()->evaluateJavaScript("ytplayer.playVideo(); null");
}

/** 
 * Pause the video.
 */

void YoutubePlayer::pause()
{
	mainFrame()->evaluateJavaScript("ytplayer.pauseVideo(); null");
}

/**
 * Seek to \a msec position in the video.
 * \a seekAhead determines if the player will seek beyond the current buffered data.
 * \warning Due to the seek resulotion provided by YouTube, this seeks to the nearest posittion.
 * \sa http://code.google.com/apis/youtube/js_api_reference.html#Playback_controls
 */

void YoutubePlayer::seek(int msec, bool seekAhead)
{
	QString js;
	if(seekAhead)
		js = "ytplayer.seekTo(%1,true); null";
	else
		js = "ytplayer.seekTo(%1,false); null";
	mainFrame()->evaluateJavaScript(js.arg(msec / 1000));
}

/** 
 * Resize the player.
 * This is not enabled on linux.
 * \sa YoutubeViewer::resizeEvent()
 * \bug this does not resize to the full width on windows.
 * it used to do that. bug looks to be on js side.
 * \bug this craches the calling application on linux( due to flash player proplem ).
 */

#ifndef FLASH_NO_RESIZE

void YoutubePlayer::resizePlayer(int width, int height)
{
	/* pixel offset to avoid scrollbars */

	int offset = 15;
	QString js = "ytplayer.width = %1; ytplayer.height = %2;";
	mainFrame()->evaluateJavaScript(js.arg( width ).arg( height ));
	setViewportSize(QSize(width + offset, height+offset ));

}

#endif

/**
 * Load the video with \a vidId, this is not playing the video.
 * \sa loadVideoById()
 * \sa http://code.google.com/apis/youtube/2.0/reference.html#youtube_data_api_tag_yt:videoid
 */

void YoutubePlayer::cueVideoById( QString vidId )
{
	QString js = "ytplayer.cueVideoById('%1');";
	js = js.arg(vidId);
	Debug << js;
	mainFrame()->evaluateJavaScript(js);
}

/**
 * Load the video with vidId, and starts playing the video.
 * \sa cueVideoById()
 * \sa http://code.google.com/apis/youtube/2.0/reference.html#youtube_data_api_tag_yt:videoid
 */

void YoutubePlayer::loadVideoById( QString vidId )
{
	if(vidId.startsWith("http://www.youtube.com/watch?v="))
		vidId.remove("http://www.youtube.com/watch?v=");
	QString js = "ytplayer.loadVideoById('%1',0,'%2');";
	js = js.arg(vidId);
	js = js.arg(m_playerQuality);
	Debug << js;
	mainFrame()->evaluateJavaScript(js);
}

/**
 * \internal Inserts this class as clientApp object on the javascript side.
 */

void YoutubePlayer::addJavaScriptObject()
{
	/* make this class an object on the js side */
	Debug << "added clientApp";
	mainFrame()->addToJavaScriptWindowObject("clientApp",this);
}

/* this slot is connected in javascript */
void YoutubePlayer::setState(int state)
{
	Debug << "YT state change:" << state;
	if(state == m_state)
		return;
	m_state = state;
	if(m_state == 0)
		emit finished();
	emit stateChanged(m_state);
}

void YoutubePlayer::setTotalTime(int time)
{
	qint64 temp = time * 1000;
	if(m_totalTime != temp)
	{
		m_totalTime = temp;
		Debug << "YT duration change:" << time;
		emit totalTimeChanged(m_totalTime);
	}
}

void YoutubePlayer::setCurrentTime(int time)
{
	qint64 temp = time * 1000;
	if(m_currentTime != temp)
	{
		m_currentTime = temp;
		Debug << "YT current pos change:" << time;
		emit currentTimeChanged(m_currentTime);
	}
}

void YoutubePlayer::handlePlayerError(int errorCode)
{
	Debug << "Youtube player error:" << errorCode;
}

void YoutubePlayer::jsDebug(QVariant value)
{
	Q_UNUSED(value);
	Debug << "debug output from javascript:" << value.toString();
}

/**
 * Sets the player quality to \a playerQuality.
 * \sa PlayerQuality
 */

void YoutubePlayer::setPlayQuality(enum PlayerQuality playerQuality)
{
	switch(playerQuality) {
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
	QString js = "ytplayer.setPlaybackQuality('%1');";
	mainFrame()->evaluateJavaScript(js.arg(m_playerQuality));
}

