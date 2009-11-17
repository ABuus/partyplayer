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