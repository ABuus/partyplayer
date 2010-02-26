#ifndef CONTROLSLIDER_H
#define CONTROLSLIDER_H

#include <QAbstractSlider>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QStyle>
#include <QMouseEvent>

class ControlSlider : public QAbstractSlider
{
	Q_OBJECT

public:
	ControlSlider(QWidget *parent);
	~ControlSlider();
private:
	QTimer m_fadeTimer;
	int m_fadeTick;
	bool m_fadeUp;
	bool handlePressed;
	QRadialGradient backgroundGradient();
	QPainterPath backgroundPath();
	QColor backgroundColor();
	QPainterPath handlePath();
	QRadialGradient handleGradient();
	QRect handleRect();
private slots:
	void updateFadeTick();
protected:
	void paintEvent(QPaintEvent *e);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
};

#endif // CONTROLSLIDER_H

