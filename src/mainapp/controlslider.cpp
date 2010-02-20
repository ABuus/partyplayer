#include "controlslider.h"

ControlSlider::ControlSlider(QWidget *parent)
	: QAbstractSlider(parent),
	m_fadeTimer(this),
	m_fadeTick(0),
	m_fadeUp(false)
{
	setAttribute(Qt::WA_Hover);
	m_fadeTimer.setInterval(20);
	setFixedHeight(12);
	connect(&m_fadeTimer,SIGNAL(timeout()),this,SLOT(updateFadeTick()));
	connect(&m_fadeTimer,SIGNAL(timeout()),this,SLOT(update()));
	connect(this,SIGNAL(sliderMoved(int)),this,SLOT(update()));
}

ControlSlider::~ControlSlider()
{

}

void ControlSlider::enterEvent(QEvent *)
{
	m_fadeUp = true;
	m_fadeTimer.start();
}

void ControlSlider::leaveEvent(QEvent *)
{
	m_fadeUp = false;
	m_fadeTimer.start();
}

void ControlSlider::updateFadeTick()
{
	if(m_fadeUp)
	{
		m_fadeTick = m_fadeTick +5;
		if(m_fadeTick > 150)
		{
			m_fadeTick = 150;
			m_fadeTimer.stop();
		}
	}
	else
	{
		m_fadeTick = m_fadeTick -10;
		if(m_fadeTick < 0)
		{
			m_fadeTick = 0;
			m_fadeTimer.stop();
		}
	}
}

void ControlSlider::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	/* draw background */
	QBrush brush(backgroundGradient());
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawPath(backgroundPath());

	/* draw handle */
	brush = handleGradient();
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawPath(handlePath());
}

QPainterPath ControlSlider::handlePath()
{
	QPainterPath path;
	path.addEllipse(handleRect());
	return path;
}

QRect ControlSlider::handleRect()
{
	int sliderPos = style()->sliderPositionFromValue(minimum(),maximum(),value(),width()-12);
	return QRect(sliderPos,0,12,12);
}

QRadialGradient ControlSlider::handleGradient()
{
	QRadialGradient gradient(0.5,0.5,1.0,0.5,0.5);
	gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
	QGradientStops stops;
	stops.append(QPair<qreal,QColor>(0.0, Qt::white));
	stops.append(QPair<qreal,QColor>(0.8, Qt::lightGray));
	stops.append(QPair<qreal,QColor>(1.0, Qt::white));
	gradient.setStops(stops);
	return gradient;
}


QRadialGradient ControlSlider::backgroundGradient()
{
	QRadialGradient gradient(0.5,0.5,1.0,0.5,0.5);
	gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
	QGradientStops stops;
	stops.append(QPair<qreal,QColor>(0.0, Qt::white));
	stops.append(QPair<qreal,QColor>(0.8, backgroundColor()));
	stops.append(QPair<qreal,QColor>(1.0, Qt::white));
	gradient.setStops(stops);
	return gradient;
}

QPainterPath ControlSlider::backgroundPath()
{
	QPainterPath path;
	path.addRoundedRect(rect(),5,5);
	return path;
}

QColor ControlSlider::backgroundColor()
{
	return QColor(m_fadeTick,0,m_fadeTick,200);
}

void ControlSlider::mousePressEvent(QMouseEvent *event)
{
	setSliderDown(true);
	if(handleRect().contains(event->pos()))
	{
		handlePressed = true;
		setSliderDown(true);
		int sliderValue = style()->sliderValueFromPosition(minimum(),maximum(),event->pos().x()-6,width()-12);
		setValue(sliderValue);
	}
}

void ControlSlider::mouseReleaseEvent(QMouseEvent *event)
{
	if(!handlePressed)
	{
		int sliderValue = style()->sliderValueFromPosition(minimum(),maximum(),event->pos().x()-6,width()-12);
		setValue(sliderValue);
	}
	handlePressed = false;
	setSliderDown(false);
}

void ControlSlider::mouseMoveEvent(QMouseEvent *event)
{
	if(isSliderDown())
	{
		int sliderValue = style()->sliderValueFromPosition(minimum(),maximum(),event->pos().x()-6,width()-12);
		setValue(sliderValue);
	}
}