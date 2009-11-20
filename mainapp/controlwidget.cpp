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

#include "controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	playState = false;
	
	setStyleSheet("QToolButton {border:none;}");
	QSize icoSize(60,60);
	playButton->setIconSize(icoSize);
	backButton->setIconSize(icoSize);
	stopButton->setIconSize(icoSize);
	forwardButton->setIconSize(icoSize);

	playButton->setIcon(QIcon(":/mainwindow/play.png"));
	backButton->setIcon(QIcon(":/mainwindow/back.png"));
	stopButton->setIcon(QIcon(":/mainwindow/stop.png"));
	forwardButton->setIcon(QIcon(":/mainwindow/forward.png"));

	connect(playButton,SIGNAL(clicked()),this,SLOT(playClicked()));
	connect(stopButton,SIGNAL(clicked()),this,SIGNAL(stop()));
	connect(backButton,SIGNAL(clicked()),this,SIGNAL(back()));
	connect(forwardButton,SIGNAL(clicked()),this,SIGNAL(forward()));
	connect(slider,SIGNAL(sliderMoved(int)), this,SIGNAL(seek(int)));
}

ControlWidget::~ControlWidget()
{

}

void ControlWidget::setPlayState(bool playing)
{
	playState = playing;
	if(playState)
	{
		playButton->setIcon(QIcon(":/mainwindow/pause.png"));
		emit play();
	}
	else
	{
		playButton->setIcon(QIcon(":/mainwindow/play.png"));
		emit pause();
	}
}

void ControlWidget::playClicked()
{
	if(playState)
		setPlayState(false);
	else
		setPlayState(true);
}