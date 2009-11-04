#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
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
