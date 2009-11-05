#ifndef WEBPLAYER_H
#define WEBPLAYER_H

#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QMouseEvent>
#include <QTimer>
#include "debug.h"

class WebPlayer : public QWebView
{
	Q_OBJECT

public:
	WebPlayer(QWidget *parent = 0);
	~WebPlayer();
	void start();
private slots:
	void click();
private:
	QTimer *startTimer;
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
};

#endif // WEBPLAYER_H
