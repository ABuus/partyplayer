#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QStandardItemModel>
#include "debug.h"
#include "ui_mainwindow.h"
#include "search.h"
#include "searchmodel.h"
#include "controlwidget.h"

class mainwindow : public QMainWindow, public Ui::mainwindowClass
{
	Q_OBJECT

public:
	mainwindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~mainwindow();

private:
	Search *search;
	QFileSystemModel *fileSysModel;
	ControlWidget *controlWidget;
	SearchModel *searchModel;
private slots:
	void querySearch();
	void clearSearch();
	void insertSearchItem(QStringList);
signals:
	void preformSearch(QString text);
};

#endif // MAINWINDOW_H
