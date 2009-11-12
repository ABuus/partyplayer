#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#ifdef Q_WS_WIN
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>
#include <Phonon/VideoWidget>
#elif defined Q_WS_X11
#include <mediaobject.h>
#include <mediasource.h>
#include <audiooutput.h>
#endif
#include <QUrl>
#include <QWebView>
#include <QTimer>
#include "playlistview.h"
#include "debug.h"

using namespace Phonon;

class Player : public QObject
{
	Q_OBJECT
public:
	Player(QObject *parent);
	~Player();
	Phonon::MediaObject * mediaObject() { return m_mediaObject; };
	QWebView * webView() { return m_webView; };
	void setPlaylist(PlaylistView *playlist) { m_playlist = playlist; };
	void setWebView(QWebView *webView) { m_webView = webView; };
	enum State { NoState, WebState, LocalState };
	//enum VideoMode { AudioOnly, VideoWidget, FullScreen };
public slots:
	void play(QUrl url);
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
	PlaylistView *m_playlist;
	QTimer *webTimer;
private slots:
	void emitPlayingState( Phonon::State pState );
signals:
	void requestNext();
	void currentSourceChanged(const QUrl &url);
	void playStateChanged(bool playing);
};

#endif // PLAYER_H
