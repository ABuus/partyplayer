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

#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

#include "ui_controlwidget.h"

class ControlWidget : public QWidget , public Ui::controlWidget
{
	Q_OBJECT

public:
	ControlWidget(QWidget *parent = 0);
	~ControlWidget();
public slots:
	void setPlayState( bool playing );
	void setTime(qint64 time) { slider->setValue(time); };
	void setTotalTime(qint64 time) { slider->setMaximum(time); };
private:
	bool playState; // true playing, false pause
private slots:
	void playClicked();
signals:
	void play();
	void pause();
	void stop();
	void back();
	void forward();
	void seek(int value);
};

#endif // CONTROLWIDGET_H
