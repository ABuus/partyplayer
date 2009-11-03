/********************************************************************************
** Form generated from reading UI file 'controlwidget.ui'
**
** Created: Tue 3. Nov 12:59:13 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLWIDGET_H
#define UI_CONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <phonon/seekslider.h>
#include <phonon/volumeslider.h>

QT_BEGIN_NAMESPACE

class Ui_ControlWidgetClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLCDNumber *lcdDisplay;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *backButton;
    QToolButton *playButton;
    QToolButton *stopButton;
    QToolButton *forwardButton;
    Phonon::SeekSlider *seekSlider;
    Phonon::VolumeSlider *volumeSlider;

    void setupUi(QWidget *ControlWidgetClass)
    {
        if (ControlWidgetClass->objectName().isEmpty())
            ControlWidgetClass->setObjectName(QString::fromUtf8("ControlWidgetClass"));
        ControlWidgetClass->resize(638, 110);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ControlWidgetClass->sizePolicy().hasHeightForWidth());
        ControlWidgetClass->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ControlWidgetClass);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lcdDisplay = new QLCDNumber(ControlWidgetClass);
        lcdDisplay->setObjectName(QString::fromUtf8("lcdDisplay"));

        horizontalLayout_2->addWidget(lcdDisplay);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        backButton = new QToolButton(ControlWidgetClass);
        backButton->setObjectName(QString::fromUtf8("backButton"));
        backButton->setMaximumSize(QSize(60, 60));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/mainwindow/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        backButton->setIcon(icon);
        backButton->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(backButton);

        playButton = new QToolButton(ControlWidgetClass);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setMaximumSize(QSize(60, 60));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/mainwindow/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon1);
        playButton->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(playButton);

        stopButton = new QToolButton(ControlWidgetClass);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setMaximumSize(QSize(60, 60));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/mainwindow/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon2);
        stopButton->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(stopButton);

        forwardButton = new QToolButton(ControlWidgetClass);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));
        forwardButton->setMaximumSize(QSize(60, 60));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/mainwindow/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        forwardButton->setIcon(icon3);
        forwardButton->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(forwardButton);


        verticalLayout->addLayout(horizontalLayout);

        seekSlider = new Phonon::SeekSlider(ControlWidgetClass);
        seekSlider->setObjectName(QString::fromUtf8("seekSlider"));
        seekSlider->setMaximumSize(QSize(16777215, 20));

        verticalLayout->addWidget(seekSlider);


        horizontalLayout_2->addLayout(verticalLayout);

        volumeSlider = new Phonon::VolumeSlider(ControlWidgetClass);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(volumeSlider->sizePolicy().hasHeightForWidth());
        volumeSlider->setSizePolicy(sizePolicy1);
        volumeSlider->setStyleSheet(QString::fromUtf8(""));
        volumeSlider->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(volumeSlider);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        retranslateUi(ControlWidgetClass);

        QMetaObject::connectSlotsByName(ControlWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *ControlWidgetClass)
    {
        ControlWidgetClass->setWindowTitle(QApplication::translate("ControlWidgetClass", "ControlWidget", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("ControlWidgetClass", "...", 0, QApplication::UnicodeUTF8));
        playButton->setText(QApplication::translate("ControlWidgetClass", "...", 0, QApplication::UnicodeUTF8));
        stopButton->setText(QApplication::translate("ControlWidgetClass", "...", 0, QApplication::UnicodeUTF8));
        forwardButton->setText(QApplication::translate("ControlWidgetClass", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ControlWidgetClass: public Ui_ControlWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
