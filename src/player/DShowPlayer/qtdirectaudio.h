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
        explicit QtDirectAudio(QObject *parent = 0);
	~QtDirectAudio();
public slots:
	bool play(const QUrl &url);
	bool play();
	void stop();
	void pause();
	void seek( qint64 time);
signals:
        /** This signal is emmitted when the the position in the stream changes. */
        void positionChanged(qint64 posision);
        /** This signal is emmitted when the duration of a stream changes */
        void durationChanged(qint64 duration);
        /** This signal is emmitted just before the current stream ends */
        void runningOut();
        /** This signal is emitted when, current stream ends */
        void finished();
protected:
	QtDirectAudioPrivate * const d_ptr;
	QtDirectAudio(QtDirectAudioPrivate &dd, QObject *parent);
private:
	Q_DECLARE_PRIVATE(QtDirectAudio);

};

#endif // QTDIRECTAUDIO_H
