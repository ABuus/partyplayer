#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#ifdef Q_WS_WIN
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>
#include <Phonon/VideoWidget>
#elif defined Q_WS_X11
#include <phonon/mediaobject.h>
#include <phonon/mediasource.h>
#include <phonon/audiooutput.h>
#endif
#include <QUrl>
#include <QWebView>
#include <QTimer>

#include "debug.h"
#include "playlist/playlist.h"

using namespace Phonon;

class Player : public QObject
{
	Q_OBJECT
public:
	Player(QObject *parent);
	~Player();
	Phonon::MediaObject * mediaObject() { return m_mediaObject; };
	QWebView * webView() { return m_webView; };
	void setWebView(QWebView *webView) { m_webView = webView; };
	void setPlaylist(Playlist *playlist) { m_playlist = playlist; };
	enum State { NoState, WebState, LocalState };
	//enum VideoMode { AudioOnly, VideoWidget, FullScreen };
public slots:
	void play(const QUrl url);
	void play() { m_mediaObject->play(); };
	void stop();
	void pause() { m_mediaObject->pause(); };
	void next();
	void previous();
	void enqueueNext();
private:
	AudioOutput *m_audioOutput;
	MediaObject *m_mediaObject;
	QWebView *m_webView;
	State m_state;
	QTimer *webTimer;
	Playlist *m_playlist;
private slots:
	void emitPlayingState( Phonon::State pState );
signals:
	void requestNext();
	void currentSourceChanged(const QUrl &url);
	void playStateChanged(bool playing);
	void timeChanged(qint64 newTime);
	void totalTimeChanged(qint64 newTotalTime);
};

#endif // PLAYER_H
