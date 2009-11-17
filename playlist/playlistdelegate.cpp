#include "playlistdelegate.h"

using namespace Playlist;

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt(option);
	QRect rect = opt.rect;

	// draw background if selected
	if(index.data(Playlist::PlayRole).toBool())
	{
		qDebug() << "paint playing";
		painter->save();
		QLinearGradient backgroundGradient(rect.topLeft().x(), rect.topLeft().y() +3, rect.bottomLeft().x(), rect.bottomLeft().y() -3 );
		backgroundGradient.setColorAt(0.0, QColor(200,200,200));
		backgroundGradient.setColorAt(0.8, QColor(100,100,100));
		backgroundGradient.setColorAt(1.0, QColor(200,200,200));
		painter->setBrush(backgroundGradient);
		painter->setPen(Qt::NoPen);
		painter->drawRect(rect.x(),rect.y(),rect.width(),rect.height()-2);
		painter->restore();
		painter->setPen(QColor(200,200,200));
	}
	// draw text
	if(option.state & QStyle::State_Selected)
		painter->setPen(QColor(200,200,200));
	else
		painter->setPen(QColor(0,0,0));
	painter->drawText(rect, Qt::AlignLeft, index.data().toString());
}
