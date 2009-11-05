#include "webplayer.h"

WebPlayer::WebPlayer(QWidget *parent)
	: QWebView(parent)
{
	startTimer = new QTimer(this);
	startTimer->setSingleShot(true);
	startTimer->setInterval(10 *1000);
	connect(startTimer,SIGNAL(timeout()),this,SLOT(click()));
}

WebPlayer::~WebPlayer()
{

}

void WebPlayer::start()
{
	startTimer->start();
}

void WebPlayer::click()
{
	Debug << "CLICK";
	
	page()->triggerAction(QWebPage::OpenLink,true);
	QMouseEvent *e = new QMouseEvent(QEvent::MouseButtonPress,QPoint(10,10),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
	mousePressEvent(e);
	mouseReleaseEvent(e);
}

void WebPlayer::mousePressEvent(QMouseEvent *e)
{
	QWebFrame *frame = page()->frameAt(e->pos());
	frame->setFocus();
	Debug << e->globalPos();
	e->accept();
	QWebView::mousePressEvent(e);
}

void WebPlayer::mouseReleaseEvent(QMouseEvent *e)
{
	Debug << e->globalPos();
	e->accept();
	QWebView::mouseReleaseEvent(e);
}