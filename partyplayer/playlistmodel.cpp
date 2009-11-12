#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent)
	: QStandardItemModel(parent)
{
	QStringList headerLabels;
	headerLabels << tr("Artist") << tr("Title") << tr("Year") << tr("Album") << tr("Track") << tr("Directory") << tr("Length");
	setHorizontalHeaderLabels(headerLabels);
}

PlaylistModel::~PlaylistModel()
{

}

Qt::DropActions PlaylistModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList PlaylistModel::mimeTypes() const
 {
     QStringList types;
     types << "application/vnd.text.list" << "text/uri-list";
     return types;
 }

QMimeData *PlaylistModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (QModelIndex index, indexes) 
	 {
		if (index.isValid()) 
		{
             QString text = data(index, Qt::DisplayRole).toString();
             stream << text;
         }
     }
     mimeData->setData("application/vnd.text.list", encodedData);
     return mimeData;
 }

bool PlaylistModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Debug << data->urls() << data->text();
    if(action == Qt::IgnoreAction)
		return true;

    if(!data->hasFormat("application/vnd.text.list"))
    {
		Debug << "return formats";
		return false;
    }

    if (column > 0)
		return false;
    int beginRow;

    if (row != -1)
		beginRow = row;
    else if (parent.isValid())
		beginRow = parent.row();
    else
		beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd())
    {
		QString text;
		stream >> text;
		newItems << text;
		++rows;
    }
    insertRows(beginRow, rows, QModelIndex());
    foreach (QString text, newItems)
    {
		QModelIndex idx = index(beginRow, 0, QModelIndex());
		setData(idx, text);
		beginRow++;
    }
    return true;
}
