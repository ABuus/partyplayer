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

#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
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

class Search : public QObject
{
	Q_OBJECT

public:
	Search(QObject *parent);
	~Search();
public slots:
	void query(QString queryString);
private:
	QNetworkAccessManager *netGetter;
private slots:
	void queryFinished(QNetworkReply *reply);
signals:
	void newSearch();
	void newItem(QStringList item);
};

#endif // SEARCH_H
