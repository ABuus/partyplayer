#include "controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	playState = false;
/*
	backButton = new QPushButton(this);
	playButton = new QPushButton(this);
	stopButton = new QPushButton(this);
	forwardButton = new QPushButton(this);
	seekSlider = new Phonon::SeekSlider(this);
*/	
//	setStyleSheet("QToolButton {width:60px; height:60; border:none;}");

	playButton->setIcon(QIcon(":/mainwindow/play.png"));
	backButton->setIcon(QIcon(":/mainwindow/back.png"));
	stopButton->setIcon(QIcon(":/mainwindow/stop.png"));
	forwardButton->setIcon(QIcon(":/mainwindow/forward.png"));

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