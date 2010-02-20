#ifndef CONTROLSLIDER_H
#define CONTROLSLIDER_H

#include <QAbstractSlider>
#include <QPainter>
#include <QStyle>
#include <QTimer>

class ControlSlider : public QAbstractSlider
{
	Q_OBJECT

public:
	ControlSlider(QWidget *parent);
	~ControlSlider();

private:
	QColor backgroundColor();
	QRadialGradient backgroundGradient();
	QPainterPath backgroundPath();
	QRadialGradient handleGradient();
	QPainterPath handlePath();
	QTimer m_fadeTimer;
	int m_fadeTick;
	bool m_fadeUp;
private slots:
	void updateFadeTick();
protected:
	void paintEvent(QPaintEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};

#endif // CONTROLSLIDER_H
