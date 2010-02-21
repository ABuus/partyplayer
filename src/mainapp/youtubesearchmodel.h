/*
** This file is part of the Party Player application.
** Copyright (C) 2009  Anders Buus
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef YOUTUBESEARCHMODEL_H
#define YOUTUBESEARCHMODEL_H

#include <QStandardItemModel>
#include <QMimeData>
#include <QByteArray>
#include <QList>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include "debug.h"

// google developer id of Anders Buus
#define dev_id "AI39si61cUdAWBI2eFoGoRKodF3zuFDyUTTFHyK_X85TDEpjIv6EgeJokdy6UiuG4bq1zri9l4gExE6oEb_xwyAXuxuUgdwdRg"

class YoutubeSearchModel : public QStandardItemModel
{
	Q_OBJECT

public:
	YoutubeSearchModel(QObject *parent);
	~YoutubeSearchModel();
public slots:
	void search(QString query, bool append = false);
	void searchMore();
private:
	QNetworkAccessManager *netGetter;
	int searchOffset;
	QString currentSearch;
private slots:
	void queryFinished(QNetworkReply *);
	void insertSearchItem(QStringList item);
protected:
	Qt::DropActions supportedDropActions() const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QStringList mimeTypes() const;
	QMimeData * mimeData(const QModelIndexList &indexes) const;
signals:
	void newItem(QStringList item);
	void searchFinished();
};

#endif // YOUTUBESEARCHMODEL_H
