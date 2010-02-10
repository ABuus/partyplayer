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

#ifndef SVGSPLASHSCREEN_H
#define SVGSPLASHSCREEN_H

#include <QSvgWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QRect>
#include <QTimer>
#include <QMouseEvent>

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
protected:
	void mousePressEvent(QMouseEvent *e);
signals:
	void finished();
};

#endif // SVGSPLASHSCREEN_H
