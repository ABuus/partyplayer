#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>
#include <QString>
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
	QWebPage *webPage;
private slots:
	void queryFinished();
signals:
	void newSearch();
	void newItem(QStringList item);
};

#endif // SEARCH_H
