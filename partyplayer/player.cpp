#include "player.h"

Player::Player(QObject *parent)
	: QObject(parent)
{
	m_state = Player::NoState;
	webTimer = new QTimer(this);
	webTimer->setSingleShot(true);

	m_audioOutput = new AudioOutput(Phonon::MusicCategory,this);
	m_mediaObject = createPlayer(Phonon::MusicCategory);
	createPath(m_mediaObject,m_audioOutput);

	connect(m_mediaObject,SIGNAL(aboutToFinish()),this,SLOT(enqueueNext()));
	connect(m_mediaObject,SIGNAL(currentSourceChanged(const Phonon::MediaSource &)),this,SIGNAL(requestNext()));
	connect(m_mediaObject,SIGNAL(stateChanged(Phonon::State , Phonon::State )),this,SLOT(emitPlayingState( Phonon::State )));
	connect(webTimer,SIGNAL(timeout()),this,SLOT(next()));
}

Player::~Player()
{
	delete m_mediaObject;
}

void Player::play(const QUrl url)
{
	bool localFile = false;
	if(url.scheme() == "http")
    {
		localFile = false;
    }
    else
    {
		localFile = true;
    }
	if(localFile)
	{
		if(m_state = Player::WebState)
		{
			m_webView->load(QUrl("http://www.youtube.com/apiplayer?version=3"));
		}
		Debug << "Local File: " << url.toString();;
		m_mediaObject->setCurrentSource(url);
		m_mediaObject->play();
		m_state = Player::LocalState;
	}
	else
	{
		if(m_state = Player::LocalState)
		{
			m_mediaObject->stop();
		}
//		QModelIndex temp = m_playlist->currentIndex();
//		QModelIndex index = m_playlist->model->index( temp.row(), 0);
//		const int time = index.data(Qt::UserRole +2).toInt() +2;  // 2 = load time offset do we need some buffering??
		m_webView->load(url);
		
//		webTimer->setInterval(time * 1000);
		webTimer->start();
		m_state = Player::WebState;
		Debug << url;
	}
}

void Player::next()
{
	/*
	QUrl url = m_playlist->next();
	play(url);
	m_playlist->selectNext();
	*/
}

void Player::previous()
{
/*
	QUrl url = m_playlist->previous();
	play(url);
	m_playlist->selectPrevious();
*/
}

void Player::stop()
{
	m_mediaObject->stop();
	m_webView->setHtml(QString());
}

void Player::enqueueNext()
{
	/*
	QUrl url = m_playlist->next();
	if(url.scheme() == "file" && m_state == Player::LocalState)
	{
		m_mediaObject->enqueue(url);
	}
	else
	{
		play(url);
	}
	m_playlist->selectNext();
	*/
}


void Player::emitPlayingState(Phonon::State pState)
{
	if(pState == Phonon::PlayingState)
		emit playStateChanged(true);
	else if(pState == Phonon::PausedState || pState == Phonon::StoppedState)
		emit playStateChanged(false);
}

