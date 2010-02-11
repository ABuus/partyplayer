#ifndef YOUTUBEPLAYER_H
#define YOUTUBEPLAYER_H

// Qt
#include <QWebPage>
#include <QWebFrame>
#include <QDebug>
#include <QFile>
#include <QWebView>
//#include "tcpserver.h"
#include "player_global.h"

#define TIMER_INTERVAL 200

class PLAYER_EXPORT YoutubePlayer : public QWebPage
{
Q_OBJECT

public:
	YoutubePlayer(QObject *parent);
	~YoutubePlayer();
public slots:
	void play();
	void pause();
	void seek(int msec);
	void resizePlayer(int w, int h);
	void cueVideoById(QString videoId);
	void loadVideoById(QString videoId);
	void addJavaScriptObject();
	/* slots called forn JS */
	void setState(int state);
	void setTotalTime(double time);
	void setCurrentTime(double time);
	void handlePlayerError(int errorCode);
	void jsDebug(QVariant value);
private:
	qint64 m_totalTime;
	qint64 m_currentTime;
	int m_state;
signals:
	void totalTimeChanged(qint64 newTotal);
	void currentTimeChanged(qint64 newCurrent);
	void stateChanged(int state);
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
	void resizeEvent(QResizeEvent *e) { 
		m_page->resizePlayer(width(),height());
		QWebView::resizeEvent(e);
	};
	void setPage(YoutubePlayer *yt_page) { m_page = yt_page; };
private:
	YoutubePlayer *m_page;
};

#endif // YOUTUBEPLAYER_H
