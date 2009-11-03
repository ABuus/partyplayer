/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue 3. Nov 19:37:57 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "playlistview.h"
#include "searchview.h"

QT_BEGIN_NAMESPACE

class Ui_mainwindowClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QTreeView *fileView;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *searchLineEdit;
    SearchView *searchView;
    PlaylistView *listView;
    QGridLayout *controlLayout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainwindowClass)
    {
        if (mainwindowClass->objectName().isEmpty())
            mainwindowClass->setObjectName(QString::fromUtf8("mainwindowClass"));
        mainwindowClass->resize(682, 403);
        centralWidget = new QWidget(mainwindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        fileView = new QTreeView(tab);
        fileView->setObjectName(QString::fromUtf8("fileView"));
        fileView->setDragEnabled(true);
        fileView->setDragDropMode(QAbstractItemView::DragOnly);
        fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        fileView->setAnimated(true);
        fileView->setHeaderHidden(true);

        gridLayout_3->addWidget(fileView, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        searchLineEdit = new QLineEdit(tab_2);
        searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));

        verticalLayout_2->addWidget(searchLineEdit);

        searchView = new SearchView(tab_2);
        searchView->setObjectName(QString::fromUtf8("searchView"));
        searchView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        searchView->setDragEnabled(true);
        searchView->setDragDropMode(QAbstractItemView::DragOnly);
        searchView->setSelectionMode(QAbstractItemView::ExtendedSelection);

        verticalLayout_2->addWidget(searchView);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        splitter->addWidget(tabWidget);
        listView = new PlaylistView(splitter);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setDragDropMode(QAbstractItemView::DropOnly);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);
        splitter->addWidget(listView);

        gridLayout->addWidget(splitter, 1, 0, 1, 1);

        controlLayout = new QGridLayout();
        controlLayout->setSpacing(0);
        controlLayout->setObjectName(QString::fromUtf8("controlLayout"));

        gridLayout->addLayout(controlLayout, 0, 0, 1, 1);

        mainwindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainwindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 682, 22));
        mainwindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainwindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainwindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainwindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        mainwindowClass->setStatusBar(statusBar);

        retranslateUi(mainwindowClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainwindowClass", "Files", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("mainwindowClass", "YouTube", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
