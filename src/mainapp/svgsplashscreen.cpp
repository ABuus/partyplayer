/*
** This file is part of the Party Player application.
** Copyright (C) 2009  Anders Buus
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
			emit finished();
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

void SvgSplashScreen::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		this->hide();
		emit finished();
	}
}