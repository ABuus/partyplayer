#ifndef YOUTUBEPLAYER_H
#define YOUTUBEPLAYER_H

// Qt
#include <QWebPage>
#include <QWebFrame>
#include <QFile>
#include <QTextStream>
#include <QWebView>
#include "../debug.h"
#include "player_global.h"

/**
 * \brief This class provides provides support for playing YouTube videos.
 * \todo Add static function to get the underlaying YouTube xml data.
 * \todo Test is we can call the underlying player directly.
 * if so make this class do that for better control of the player( QtWebKit cache ).
 */

class PLAYER_EXPORT YoutubePlayer : public QWebPage
{
Q_OBJECT

public:
	enum PlayerQuality {
		Low = 0,
		Medium,
		High,
		Hd720,
		Standard
	};
	YoutubePlayer(QObject *parent);
	~YoutubePlayer();
	bool playUrl(const QUrl url);
public slots:
	void play();
	void pause();
#ifndef FLASH_NO_RESIZE
	void resizePlayer(int w, int h);
#endif
	/* slots called forn JS */
	void setState(int state);
	void setTotalTime(int time);
	void setCurrentTime(int time);
	void handlePlayerError(int errorCode);
	void jsDebug(QVariant value);
	/**/
	void seek(int msec, bool seekAhead = true);
	void setPlayQuality(enum PlayerQuality playerQuality = YoutubePlayer::Standard);
	void cueVideoById(QString videoId);
	void loadVideoById(QString videoId);
private slots:
	void addJavaScriptObject();
private:
	qint64 m_totalTime;
	qint64 m_currentTime;
	int m_state;
	QString m_playerQuality;
signals:
	/** This signal is emitted when the duration changes. */
	void totalTimeChanged(qint64 newTotal);
	/** This signal is emitted when the position changes. */
	void currentTimeChanged(qint64 newCurrent);
	/** 
	 * This signal is emitted when the player change state.
	 * \todo make Player namespace and enum State and fix these together with GstPlayer::stateChanged()
	 */
	void stateChanged(int state);
	/**	This signal is emmited when the stream ends. */
	void finished();
};

class PLAYER_EXPORT YoutubeViewer : public QWebView
{
	Q_OBJECT
public:
	YoutubeViewer(QWidget *parent)
		: QWebView(parent)
	{
		m_page = new YoutubePlayer(this);
	}
#ifndef FLASH_NO_RESIZE
	void resizeEvent(QResizeEvent *e) { 
		m_page->resizePlayer(width(),height());
		QWebView::resizeEvent(e);
	};
#endif
	void setPage(YoutubePlayer *yt_page) { m_page = yt_page; };
private:
	YoutubePlayer *m_page;
};

#endif // YOUTUBEPLAYER_H
