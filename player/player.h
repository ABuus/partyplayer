#ifndef PLAYER_H
#define PLAYER_H

// Qt
#include <QObject>
#include <QUrl>
#include <QDebug>
#include <QTimer>
// gstreamer
#include <glib-object.h>
#include <gst/gst.h>
#include "player_global.h"


class PLAYER_EXPORT Player : public QObject
{
	Q_OBJECT
public:
	Player(QObject *parent = 0);
	~Player();
public slots:
	void playUrl(const QUrl &url);
	void seek( int time );
	void play();
	void stop();
	void pause();
private:
	GstElement *m_pipeline;
	GstElement *m_sink;
	QTimer m_playTimer;
	qint64 m_totaltime;
	private slots:
	void getTime();
	void getTotalTime();
signals:
	void timeChanged( qint64 pos );
	void totalTimeChanged( qint64 totalTime );
};

#endif // PLAYER_H
