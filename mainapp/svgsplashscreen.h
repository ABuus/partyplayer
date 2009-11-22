#ifndef SVGSPLASHSCREEN_H
#define SVGSPLASHSCREEN_H

#include <QSvgWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QRect>
#include <QTimer>
#include <QDebug>

class SvgSplashScreen : public QSvgWidget
{
	Q_OBJECT

public:
	SvgSplashScreen(const QString &file, QWidget *parent = 0);
	~SvgSplashScreen();
public slots:
	void fadeIn();
	void fadeOut();
private:
	QTimer m_fadeTimer;
	bool m_fadeingIn;
	qreal opa;
private slots:
	void setOpacity();
};

#endif // SVGSPLASHSCREEN_H
