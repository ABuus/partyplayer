#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
/*
#include <QPushButton>
#ifdef Q_WS_WIN
#include <SeekSlider>
#else
#include <seekslider.h>
#endif
*/
#include "ui_controlwidget.h"

class ControlWidget : public QWidget , public Ui::controlWidget
{
	Q_OBJECT

public:
	ControlWidget(QWidget *parent = 0);
	~ControlWidget();
//	Phonon::SeekSlider *seekSlider;
public slots:
	void setPlayState( bool playing );
private:
	bool playState; // true playing, false pause
/*
	QPushButton *playButton;
	QPushButton *stopButton;
	QPushButton *backButton;
	QPushButton *forwardButton;
*/
private slots:
	void playClicked();
signals:
	void play();
	void pause();
	void stop();
	void back();
	void forward();
};

#endif // CONTROLWIDGET_H
