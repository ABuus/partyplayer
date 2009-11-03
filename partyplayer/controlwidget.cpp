#include "controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	playState = false;
	
//	setStyleSheet("QToolButton {width:60px; height:60; border:none;}");
	connect(playButton,SIGNAL(clicked()),this,SLOT(playClicked()));
	connect(stopButton,SIGNAL(clicked()),this,SIGNAL(stop()));
	connect(backButton,SIGNAL(clicked()),this,SIGNAL(back()));
	connect(forwardButton,SIGNAL(clicked()),this,SIGNAL(forward()));
	
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