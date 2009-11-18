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
