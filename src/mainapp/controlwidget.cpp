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
	QGridLayout *gridLayout = new QGridLayout(this);
	QHBoxLayout *vBoxLayout = new QHBoxLayout;
	m_previousButton = new ControlButton(ControlButton::PreviousStyle,this);
	m_playPauseButton = new ControlButton(ControlButton::PlayStyle,this);
	m_stopButton = new ControlButton(ControlButton::StopStyle,this);
	m_nextButton = new ControlButton(ControlButton::NextStyle,this);
	m_slider = new ControlSlider(this);

	vBoxLayout->addWidget(m_previousButton);
	vBoxLayout->addSpacing(15);
	vBoxLayout->addWidget(m_playPauseButton);
	vBoxLayout->addSpacing(15);
	vBoxLayout->addWidget(m_stopButton);
	vBoxLayout->addSpacing(15);
	vBoxLayout->addWidget(m_nextButton);
	gridLayout->addLayout(vBoxLayout,0,0);
	gridLayout->addWidget(m_slider,1,0);
	
	setFixedWidth( (m_previousButton->width() * 4) + 45);
//	setMinimumWidth( (m_previousButton->width() * 4) + 45 );



	connect(m_previousButton,SIGNAL(clicked()),this,SIGNAL(back()));
	connect(m_playPauseButton,SIGNAL(clicked()),this,SLOT(playClicked()));
	connect(m_stopButton,SIGNAL(clicked()),this,SIGNAL(stop()));
	connect(m_nextButton,SIGNAL(clicked()),this,SIGNAL(forward()));
	connect(m_slider,SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));
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

void ControlWidget::setTime(qint64 time)
{
	m_slider->setValue((int)time);
}

void ControlWidget::setTotalTime(qint64 time)
{
	m_slider->setMaximum((int)time);
}

void ControlWidget::onSliderMoved(int value)
{
	emit seek((qint64)value);
}
