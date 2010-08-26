#ifndef QTDIRECTAUDIO_H
#define QTDIRECTAUDIO_H

#include <qobject.h>
#include <qurl.h>
#include "../player_global.h"

class QtDirectAudioPrivate;

class PLAYER_EXPORT QtDirectAudio : public QObject
{
	Q_OBJECT
public:
	explicit QtDirectAudio(QObject *parent);
	~QtDirectAudio();
public slots:
	bool play(const QUrl &url);
	bool play();
	void stop();
	void pause();
	void seek( qint64 time);
protected:
	QtDirectAudioPrivate * const d_ptr;
	QtDirectAudio(QtDirectAudioPrivate &dd, QObject *parent);
private:
	Q_DECLARE_PRIVATE(QtDirectAudio);
signals:
	void durationChanged(qint64 duration);
	void positionChanged(qint64 posision);
	void runningOut();
	void finished();
};

#endif // QTDIRECTAUDIO_H
