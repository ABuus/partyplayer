#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include "ui_controlwidget.h"

class ControlWidget : public QWidget, public Ui::ControlWidgetClass
{
	Q_OBJECT

public:
	ControlWidget(QWidget *parent = 0);
	~ControlWidget();
public slots:
	void setPlayState( bool playing );
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
};

#endif // CONTROLWIDGET_H
