#include "controlbutton.h"
#include <QLabel>

ControlButton::ControlButton(enum ButtonStyle style, QWidget *parent)
	:QAbstractButton(parent),
	m_style(style),
	m_fadeTimer(this),
	m_fadeTick(0),
	m_fadeUp(false)
{
	setAttribute(Qt::WA_Hover);
	setFixedSize(60,60);
	m_fadeTimer.setInterval(20);
	connect(&m_fadeTimer,SIGNAL(timeout()),this,SLOT(updateFadeTick()));
	connect(&m_fadeTimer,SIGNAL(timeout()),this,SLOT(update()));
}

void ControlButton::setButtonStyle(enum ButtonStyle style)
{
	if(m_style == style)
		return;
	m_style = style;
	update();
}

void ControlButton::enterEvent(QEvent *)
{
	m_fadeUp = true;
	m_fadeTimer.start();
}

void ControlButton::leaveEvent(QEvent *)
{
	m_fadeUp = false;
	m_fadeTimer.start();
}

void ControlButton::updateFadeTick()
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

void ControlButton::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	/* draw background */
	QBrush brush(backgroundGradient());
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);

	painter.drawPath(backgroundPath());

	/* draw foreground */
	painter.setBrush(QColor(127,127,127));
	painter.drawPath(forgroundPath());
	if( m_style == ControlButton::PreviousStyle)
	{
		QPainterPath path;
		QRectF fgRectF = rect();
		fgRectF.adjust(15.0,15.0,-15.0,-15.0);
		path.addRoundedRect(fgRectF.topLeft().x()-5,fgRectF.topLeft().y() +10,5,10,2,2);
		painter.drawPath(path);
	}
	else if( m_style == ControlButton::NextStyle)
	{
		QPainterPath path;
		QRectF fgRectF = rect();
		fgRectF.adjust(15.0,15.0,-15.0,-15.0);
		path.addRoundedRect(fgRectF.topRight().x(),fgRectF.topRight().y() +10,5,10,2,2);
		painter.drawPath(path);
	}
}

QPainterPath ControlButton::forgroundPath()
{
	QPainterPath path;
	QPolygonF polygon;
	QRectF fgRectF = rect();
	fgRectF.adjust(15.0,15.0,-15.0,-15.0);

	switch(m_style) {
	case ControlButton::PreviousStyle:
		polygon << fgRectF.topRight();
		polygon << QPoint(fgRectF.bottomLeft().x() -5, fgRectF.bottomLeft().y() -15);
		polygon << fgRectF.bottomRight();
		path.addPolygon(polygon);
		path.closeSubpath();
		break;
	case ControlButton::PlayStyle:
		polygon << fgRectF.topLeft();
		polygon << QPoint(fgRectF.bottomRight().x() +5, fgRectF.bottomRight().y() -15);
		polygon << fgRectF.bottomLeft();
		path.addPolygon(polygon);
		path.closeSubpath();
		break;
	case ControlButton::PauseStyle:
		path.addRoundedRect(fgRectF.x()+2,fgRectF.y(),fgRectF.width()-20,fgRectF.height(),2,2);
		path.addRoundedRect(fgRectF.x()+18,fgRectF.y(),fgRectF.width()-20,fgRectF.height(),2,2);
		break;
	case ControlButton::NextStyle:
		polygon << fgRectF.topLeft();
		polygon << QPoint(fgRectF.bottomRight().x() +5, fgRectF.bottomRight().y() -15);
		polygon << fgRectF.bottomLeft();
		path.addPolygon(polygon);
		path.closeSubpath();
		break;
	case ControlButton::StopStyle:
		path.addRoundedRect(fgRectF,width() / 15, width() / 15);
		break;
	}
	return path;
}

QPainterPath ControlButton::backgroundPath()
{
	QPainterPath path;
	QRectF outerRectF = rect();
	QRectF innerRectF = rect();
	qreal offset = width() / 7.5;
	qreal inv_offset = offset / -1;
	innerRectF.adjust(offset,offset,inv_offset,inv_offset);
	path.moveTo(outerRectF.center());
	path.arcTo(outerRectF,0.0,360.0);
	path.arcTo(innerRectF,0.0,360.0);
	return path;
}

QRadialGradient ControlButton::backgroundGradient()
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

QColor ControlButton::backgroundColor()
{
	return QColor(m_fadeTick,0,m_fadeTick,200);
}
