#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>
#include <QWebPage>
#include <QWebFrame>
#include <QWebView>
#include <QUrl>
#include "debug.h"

using namespace Phonon;

class Player : public QObject
{
	Q_OBJECT

public:
	Player(QObject *parent);
	~Player();
	Phonon::MediaObject * mediaObject() { return m_mediaObject; };
	enum State { WebState, LocalState };
public slots:
	void play(QUrl url,bool localFile);
	void play() { m_mediaObject->play(); };
	void stop();
	void pause() { m_mediaObject->pause(); };
	void next();
	void previous();
	void loadNext();
	void setNext(QUrl url,bool localFile);
	void setCurrentSource(const Phonon::MediaSource &);
private:
	AudioOutput *m_audioOutput;
	MediaObject *m_mediaObject;
	QWebPage *webPlayer;
	QWebView *webView;
	State m_state;
	QUrl nextUrl;
	bool nextIsLocal;
signals:
	void requestNext();
	void currentSourceChanged(const QUrl &url);
};

#endif // PLAYER_H
