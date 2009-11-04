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
}

Player::~Player()
{
	delete m_mediaObject;
}

void Player::play(QUrl url, bool localFile)
{
	if(localFile)
	{
		webPlayer->mainFrame()->setHtml(QString());
		Debug << "play:" << url.toString();
		m_mediaObject->setCurrentSource(url);
		m_mediaObject->play();
	}
	else
	{
		stop();
		webPlayer->mainFrame()->load(url);
		Debug << url;
	}
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