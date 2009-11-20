#include "playlistdelegate.h"

using namespace Playlist;

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 opt(option);
	QRect rect = opt.rect;
	bool playing = index.data(PlayRole).toBool();
	// paint background if item is playing
	if(playing)
	{
		painter->save();
		QLinearGradient backgroundGradient(rect.topLeft().x(), rect.topLeft().y() +3, rect.bottomLeft().x(), rect.bottomLeft().y() -3 );
		backgroundGradient.setColorAt(0.0, QColor(200,200,200));
		backgroundGradient.setColorAt(0.8, QColor(100,100,100));
		backgroundGradient.setColorAt(1.0, QColor(200,200,200));
		painter->setBrush(backgroundGradient);
		painter->setPen(Qt::NoPen);
		painter->drawRect(rect.x(),rect.y(),rect.width(),rect.height()-2);
		painter->restore();
	}
	// draw text
	if(option.state & QStyle::State_Selected || playing)
		painter->setPen(QColor(0,0,0));
	else
		painter->setPen(QColor(175,175,175));
	rect.adjust(2,2,-2,-2);
	int column = index.column() +1;
	if(column < Year || Bitrate < column )
		painter->drawText(rect, Qt::AlignLeft, index.data().toString());
	else
		painter->drawText(rect, Qt::AlignCenter, index.data().toString());
}
