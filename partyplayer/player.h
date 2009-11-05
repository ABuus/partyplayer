#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>
#include <QUrl>
#include <QTimer>
#include "playlistview.h"
#include "debug.h"
#include "webplayer.h"

using namespace Phonon;

class Player : public QObject
{
	Q_OBJECT
public:
	Player(QObject *parent);
	~Player();
	Phonon::MediaObject * mediaObject() { return m_mediaObject; };
	void setPlaylist(PlaylistView *playlist) { m_playlist = playlist; };
	enum State { NoState, WebState, LocalState };
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
	WebPlayer *webView;
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
