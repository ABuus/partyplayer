#ifndef CONTROLBUTTON_H
#define CONTROLBUTTON_H

#include <QAbstractButton>
#include <QPainter>
#include <QTimer>
#include "debug.h"

class ControlButton : public QAbstractButton
{
Q_OBJECT
public:
	enum ButtonStyle {
		PlayStyle = 0,
		StopStyle,
		PauseStyle,
		PreviousStyle,
		NextStyle
	} ButtonStyle;

	ControlButton(enum ButtonStyle style, QWidget *parent = 0);

	void setButtonStyle(enum ButtonStyle style);
//	QSize minimumSizeHint() { return QSize(50,50); };
signals:

public slots:
private:
	int m_style;
	QTimer m_fadeTimer;
	int m_fadeTick;
	bool m_fadeUp;
	QColor backgroundColor();
	QRadialGradient backgroundGradient();
	QPainterPath backgroundPath();
	QPainterPath forgroundPath();
private slots:
	void updateFadeTick();
protected:
	void paintEvent(QPaintEvent *e);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
};

#endif // CONTROLBUTTON_H
