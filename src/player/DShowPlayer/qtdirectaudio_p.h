#ifndef QTDIRECTAUDIO_P_H
#define QTDIRECTAUDIO_P_H

#include "qtdirectaudio.h"
#include <qobject.h>
#include <qurl.h>
#include <qtimer.h>
// move these around, and you will be windows fucked.
#include <atlbase.h>
#include <windows.h>
#include <dshow.h>

#define TIMER_INTERVAL 50
#define DSHOWTIMETOSEC(time) ((time) / (1e7))

class QtDirectAudioPrivate : public QObject
{
	Q_OBJECT
	Q_DECLARE_PUBLIC(QtDirectAudio)
public:
	QtDirectAudioPrivate(QtDirectAudio *parent);	
	~QtDirectAudioPrivate();
	QtDirectAudio * const q_ptr;
	bool play(const QUrl &url);
	bool play();
	void stop();
	void seek(qint64 pos);
private:
	void clearFilters();
	IMediaControl *m_mediaControl;
	IGraphBuilder *m_mediaGraph;
	IMediaSeeking *m_mediaSeeking;
	QTimer *playTimer;
	bool durationSet;
	qint64 m_duration;
	qint64 m_posision;
	bool m_canRunout;
	bool m_canFinish;
private slots:
	void getTime();
};

#endif // QTDIRECTAUDIO_P_H