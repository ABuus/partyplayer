#include "qtdirectaudio.h"
#include "qtdirectaudio_p.h"

#include <qurl.h>
#include <qdebug.h>
#include <qstring.h>
#include <string.h>
#include <atlstr.h>
#include <iostream>
using namespace std;

/*----------  PUBLIC CLASS	 ----------*/

QtDirectAudio::QtDirectAudio(QObject *parent)
	:QObject(parent),
	d_ptr(new QtDirectAudioPrivate(this))
{}

QtDirectAudio::QtDirectAudio(QtDirectAudioPrivate &dd, QObject *parent)
	:QObject(parent),
	d_ptr(&dd)
{}

QtDirectAudio::~QtDirectAudio()
{}

bool QtDirectAudio::play(const QUrl &url)
{
	Q_D(QtDirectAudio);
	return d->play(url);
}

bool QtDirectAudio::play()
{
	Q_D(QtDirectAudio);
	return d->play();
}

void QtDirectAudio::stop()
{
	Q_D(QtDirectAudio);
	d->stop();
}

void QtDirectAudio::pause()
{}

void QtDirectAudio::seek(qint64 time)
{
	Q_D(QtDirectAudio);
	d->seek(time);
}

/*----------  PRIVATE CLASS	 ----------*/

QtDirectAudioPrivate::QtDirectAudioPrivate(QtDirectAudio *parent)
	:q_ptr(parent),
        playTimer(new QTimer(this)),
	durationSet(false)
{
	HRESULT hr;
	// Initialise DirectShow
	hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder, (void**) &m_mediaGraph);
	if( SUCCEEDED(hr) )
		qDebug() << "Direct show initalised";
	else
		qDebug() << "Direct show failed to initalised";
	
	// Initialise IMediaControl
	hr = m_mediaGraph->QueryInterface(IID_IMediaControl,(void**)&m_mediaControl);
	if( SUCCEEDED(hr) )
		qDebug() << "Media Controller initalised";
	else
		qDebug() << "Media Controller failed to initalised";

	// Initialise IMediaSeeking
	hr = m_mediaGraph->QueryInterface(IID_IMediaSeeking,(void**)&m_mediaSeeking);
	if( SUCCEEDED(hr) )
		qDebug() << "Media Seeking initalised";
	else
		qDebug() << "Media Seeking failed to initalised";
	m_mediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);

	// setup the play timer
	playTimer->setInterval(TIMER_INTERVAL);
	QObject::connect(playTimer,SIGNAL(timeout()),this,SLOT(getTime()));
}

QtDirectAudioPrivate::~QtDirectAudioPrivate()
{
	CoUninitialize();
}

bool QtDirectAudioPrivate::play(const QUrl &url)
{
	if(!url.isValid())
	{
		qDebug() << "invalid url" << url.toString();
		return false;
	}

	stop();

	const wstring input = url.toString().toStdWString();
	const wchar_t *fileName = input.c_str();

	HRESULT hr;
	hr = m_mediaGraph->RenderFile(fileName,NULL);
	if(SUCCEEDED(hr))
	{
		durationSet = false;
		m_mediaControl->Run();
		playTimer->start();
		qDebug() << "running media controller";
		return true;
	}
	else
	{
		qDebug() << "failed to render file";
		return false;
	}
	return false;
}

bool QtDirectAudioPrivate::play()
{
	HRESULT hr;
	hr = m_mediaControl->Run();
	if(SUCCEEDED(hr)) {
		playTimer->start();
		return true;
	}
	return false;
}

void QtDirectAudioPrivate::stop()
{
	m_mediaControl->Stop();
	playTimer->stop();
	durationSet = false;
	m_canRunout = true;
	m_canFinish = true;
	clearFilters();
}

void QtDirectAudioPrivate::clearFilters()
{
	ULONG cFetched;
	IEnumFilters *filters;
	IBaseFilter *filter;
	m_mediaGraph->EnumFilters(&filters);
	
	while(filters->Next(1,&filter,&cFetched) == S_OK)
	{
		FILTER_INFO FilterInfo;
		if(FAILED(filter->QueryFilterInfo(&FilterInfo)))
		{
			qDebug() << "filter release error";
			continue;
		}
		if(FilterInfo.pGraph != NULL)
			FilterInfo.pGraph->Release();
		m_mediaGraph->RemoveFilter(filter);
		filter->Release();
		qDebug() << "filter released";
	}
}

void QtDirectAudioPrivate::seek(qint64 pos)
{
	playTimer->stop();
	HRESULT hr;
	qint64 stop;
	hr = m_mediaSeeking->GetDuration(&stop);
	if(FAILED(hr))
		return;
	m_mediaSeeking->SetPositions(&pos,AM_SEEKING_AbsolutePositioning,&stop,AM_SEEKING_AbsolutePositioning);
	playTimer->start();
}

void QtDirectAudioPrivate::getTime()
{
	Q_Q(QtDirectAudio);
	qint64 duration;
	qint64 posision;
	HRESULT hr;

	// emit time signals
	if(!durationSet)
	{
		hr = m_mediaSeeking->GetDuration(&duration);
		if(SUCCEEDED(hr)) {
			m_duration = duration;
			emit q->durationChanged(duration);
			durationSet = true;
		}
		else {
			qDebug() << "Failed to set duration";
		}
	}

	hr = m_mediaSeeking->GetCurrentPosition(&posision);
	if(SUCCEEDED(hr)) {
		m_posision = posision;
		emit q->positionChanged(posision);
	}
	else {
		qDebug() << "Failed to set posision";
	}
	
	// check for run out
	if((DSHOWTIMETOSEC(m_duration) - DSHOWTIMETOSEC(m_posision)) < 1.0)
	{
		if(m_canRunout)
		{
			qDebug() << "running out" << (m_duration - m_posision);
			emit q->runningOut();
			m_canRunout = false;
		}
		if((DSHOWTIMETOSEC(m_duration) - DSHOWTIMETOSEC(m_posision)) <= 0.0 && m_canFinish)
		{
			qDebug() << "finished";
			emit q->finished();
			m_canFinish = false;
		}
	}
}
