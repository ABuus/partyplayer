#include "player.h"

Player::Player(QObject *parent)
	: QObject(parent)
{
	m_audioOutput = new AudioOutput(Phonon::MusicCategory,this);
	m_mediaObject = createPlayer(Phonon::MusicCategory);
	createPath(m_mediaObject,m_audioOutput);
	webPlayer = new QWebPage(this);
	webView = new QWebView();
	webView->setPage(webPlayer);
	connect(m_mediaObject,SIGNAL(aboutToFinish()),this,SLOT(loadNext()));
	connect(m_mediaObject,SIGNAL(currentSourceChanged(const Phonon::MediaSource &)),
		this,SLOT(setCurrentSource(const Phonon::MediaSource &)));
}

Player::~Player()
{
	delete m_mediaObject;
}

void Player::play(QUrl url, bool localFile)
{
	if(localFile)
	{
		if(m_state = Player::WebState)
		{
			webPlayer->mainFrame()->setHtml(QString());
		}

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
		webPlayer->mainFrame()->load(url);
		m_state = Player::WebState;
		Debug << url;
	}
	emit requestNext();
}

void Player::next()
{
	Debug << "unimplemented";
}

void Player::previous()
{
	Debug << "unimplemented";
}

void Player::stop()
{
	m_mediaObject->stop();
	webPlayer->mainFrame()->setHtml(QString());
}

void Player::loadNext()
{
	if(nextIsLocal && m_state == Player::LocalState)
	{
		m_mediaObject->enqueue(nextUrl);
		emit requestNext();
	}
}

void Player::setNext(QUrl url,bool localFile)
{
	nextUrl = url;
	nextIsLocal = localFile;
}

void Player::setCurrentSource(const Phonon::MediaSource &source)
{
	emit currentSourceChanged(source.url());
}