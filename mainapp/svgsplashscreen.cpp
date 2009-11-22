#include "svgsplashscreen.h"

SvgSplashScreen::SvgSplashScreen(const QString &file, QWidget *parent)
	: QSvgWidget(file,parent),
	opa(0.0)
{
	setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);

	// move to center of screen
	QRect desktopRect = QApplication::desktop()->screenGeometry();
	QRect splashRect = rect();
	int xOffset = ( splashRect.center().x() / 2 );
	int yOffset = ( splashRect.center().y() / 2 );
	splashRect.adjust(xOffset,yOffset,0,0);
	move(desktopRect.center() - splashRect.center());

	m_fadeTimer.setInterval(10);
	connect(&m_fadeTimer,SIGNAL(timeout()),this,SLOT(setOpacity()));
	setWindowOpacity(opa);
}

SvgSplashScreen::~SvgSplashScreen()
{

}

void SvgSplashScreen::setOpacity()
{
	if(m_fadeingIn)
	{
		setWindowOpacity(opa);
		opa = opa + 0.02;
		if(opa >= 1)
			m_fadeTimer.stop();
	}
	else
	{
		setWindowOpacity(opa);
		opa = opa - 0.02;
		if(opa <= 0)
		{
			m_fadeTimer.stop();
			hide();
		}
	}
}

void SvgSplashScreen::fadeIn()
{
	m_fadeingIn = true;
	m_fadeTimer.start();
}

void SvgSplashScreen::fadeOut()
{
	m_fadeingIn = false;
	m_fadeTimer.start();
}