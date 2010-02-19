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
	: QWidget(parent),
	m_playState(0)
{
	QHBoxLayout *vBoxLayout = new QHBoxLayout(this);
	m_previousButton = new ControlButton(ControlButton::PreviousStyle,this);
	m_playPauseButton = new ControlButton(ControlButton::PlayStyle,this);
	m_stopButton = new ControlButton(ControlButton::StopStyle,this);
	m_nextButton = new ControlButton(ControlButton::NextStyle,this);

	vBoxLayout->addWidget(m_previousButton);
	vBoxLayout->addWidget(m_playPauseButton);
	vBoxLayout->addWidget(m_stopButton);
	vBoxLayout->addWidget(m_nextButton);

	setFixedWidth( (m_previousButton->width() * 4) + 4);
	setMinimumWidth( (m_previousButton->width() * 4) + 4 );

	m_slider = new QSlider(Qt::Horizontal,this);

	m_slider->hide();

	connect(m_previousButton,SIGNAL(clicked()),this,SIGNAL(back()));
	connect(m_playPauseButton,SIGNAL(clicked()),this,SLOT(playClicked()));
	connect(m_stopButton,SIGNAL(clicked()),this,SIGNAL(stop()));
	connect(m_nextButton,SIGNAL(clicked()),this,SIGNAL(forward()));
	connect(m_slider,SIGNAL(sliderMoved(int)),this,SIGNAL(seek(int)));

/*	setStyleSheet("QToolButton {border:none;}");
	QSize icoSize(60,60);
	playButton->setIconSize(icoSize);
	backButton->setIconSize(icoSize);
	stopButton->setIconSize(icoSize);
	forwardButton->setIconSize(icoSize);

	playButton->setIcon(QIcon(":/mainwindow/play.png"));
	backButton->setIcon(QIcon(":/mainwindow/reverse.png"));
	stopButton->setIcon(QIcon(":/mainwindow/stop.png"));
	forwardButton->setIcon(QIcon(":/mainwindow/next.png"));

	connect(playButton,SIGNAL(clicked()),this,SLOT(playClicked()));
	connect(stopButton,SIGNAL(clicked()),this,SIGNAL(stop()));
	connect(backButton,SIGNAL(clicked()),this,SIGNAL(back()));
	connect(forwardButton,SIGNAL(clicked()),this,SIGNAL(forward()));
	connect(slider,SIGNAL(sliderMoved(int)), this,SIGNAL(seek(int)));
*/
}

ControlWidget::~ControlWidget()
{

}

/*
 * state 0=stoped 1=playing 2=paused
 */

void ControlWidget::setPlayState(int state)
{
	if(state == m_playState)
	{
		return;
	}
	m_playState = state;
	Debug << "State" << state;
	// stoped state
	if(m_playState == 0)
	{
		m_playPauseButton->setButtonStyle(ControlButton::PlayStyle);
		connect(m_playPauseButton,SIGNAL(clicked()),this,SIGNAL(play()));
		disconnect(m_playPauseButton,SIGNAL(clicked()),this,SIGNAL(pause()));
		m_stopButton->setEnabled(false);
	}
	// playing state
	else if(m_playState== 1)
	{
		m_playPauseButton->setButtonStyle(ControlButton::PauseStyle);
		connect(m_playPauseButton,SIGNAL(clicked()),this,SIGNAL(pause()));
		disconnect(m_playPauseButton,SIGNAL(clicked()),this,SIGNAL(play()));
		m_stopButton->setEnabled(true);
	}
	// paused state
	else if(m_playState == 2)
	{
		m_playPauseButton->setButtonStyle(ControlButton::PlayStyle);
		connect(m_playPauseButton,SIGNAL(clicked()),this,SIGNAL(play()));
		disconnect(m_playPauseButton,SIGNAL(clicked()),this,SIGNAL(pause()));
		m_stopButton->setEnabled(true);
	}

}

void ControlWidget::playClicked()
{
	if(m_playState == 2)
	{
		setPlayState(1);
		emit play();
	}
	else
	{
		setPlayState(2);
		emit pause();
	}
}
