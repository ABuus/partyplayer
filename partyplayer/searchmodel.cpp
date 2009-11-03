#include "searchmodel.h"

SearchModel::SearchModel(QObject *parent)
	: QStandardItemModel(parent)
{

}

SearchModel::~SearchModel()
{

}

Qt::DropActions SearchModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

Qt::ItemFlags SearchModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if(index.isValid())
        return Qt::ItemIsDragEnabled | defaultFlags;
    else
        return Qt::NoItemFlags;
}

QStringList SearchModel::mimeTypes() const
{
	QStringList types;
	types << "text/uri-list";
	return types;
}

QMimeData *SearchModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;
	QList<QUrl> urlList;
	QString text;

	foreach (QModelIndex index, indexes) 
	{
		if (index.isValid()) 
		{
			text.append( data(index, Qt::DisplayRole).toString() + ";");
			urlList.append( QUrl( data(index, Qt::UserRole +1).toUrl() ));
		}
	}

	mimeData->setText(text);
	mimeData->setUrls(urlList);
        Debug << mimeData->urls();
	return mimeData;
}

