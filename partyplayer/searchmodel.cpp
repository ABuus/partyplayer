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
/*
Qt::ItemFlags SearchModel::flags(const QModelIndex &index) const
{
	if(index.isValid())
         return Qt::ItemIsDragEnabled;
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
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes) 
	{
		if (index.isValid()) 
		{
			QString text = data(index, Qt::DisplayRole).toString();
			urlList.append( QUrl( data(index, Qt::UserRole +1).toUrl() ));
			stream << text;
		}
	}

	mimeData->setData("text/uri-list", encodedData);
	mimeData->setUrls(urlList);
	return mimeData;
}
*/