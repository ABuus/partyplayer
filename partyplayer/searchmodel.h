#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QStandardItemModel>
#include <QMimeData>
#include <QByteArray>
#include <QList>
#include <QUrl>
#include <QDataStream>

class SearchModel : public QStandardItemModel
{
	Q_OBJECT

public:
	SearchModel(QObject *parent);
	~SearchModel();

private:
protected:
	Qt::DropActions supportedDropActions() const;
	/*
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QStringList mimeTypes() const;
	QMimeData * mimeData(const QModelIndexList &indexes) const;
	*/
};

#endif // SEARCHMODEL_H
