#include "localitem.h"
#include <typeinfo>
#include <fileref.h>
#include <tag.h>
#include <audioproperties.h>
#include <mpegfile.h>
#include <tlist.h>
#include <id3v2frame.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <tbytevector.h>

namespace PlaylistItem {

using namespace TagLib;

LocalItem::LocalItem(const QString &url,QObject *parent)
	: PlaylistItem(url,parent)
{
}

LocalItem::LocalItem(const QUrl &url,QObject *parent)
	:PlaylistItem(url,parent)
{
}

LocalItem::~LocalItem()
{
}

void LocalItem::fetchData()
{
	QByteArray byteArray = url().toLocal8Bit();
	if(byteArray.startsWith(FILE_MARCO))
	{
		byteArray.remove(0,sizeof(FILE_MARCO)-1);
	}

	FileName tFileName = byteArray.constData();
	FileRef tFileRef(tFileName,true);
	File *tFile = tFileRef.file();

	if(!tFileRef.isNull() && tFileRef.tag())
	{
		bool image = false;
		if(typeid(*tFile) == typeid(MPEG::File) )
		{
			image = readMpegImage();
		}

		Tag *tTag = tFileRef.tag();
		m_artist = QString::fromUtf8(tTag->artist().toCString(true));		
		m_title = QString::fromUtf8(tTag->title().toCString(true));		
		m_album = QString::fromUtf8(tTag->album().toCString(true));		
		m_comment = QString::fromUtf8(tTag->comment().toCString(true));		
		m_genre = QString::fromUtf8(tTag->genre().toCString(true));
		m_year = tTag->year();
		m_track = tTag->track();
		
		if(tFileRef.audioProperties())
		{
			AudioProperties *tAudioProperties = tFileRef.audioProperties();
			m_length = tAudioProperties->length();
			m_bitrate = tAudioProperties->bitrate();
			m_samplerate = tAudioProperties->sampleRate();
			m_channels = tAudioProperties->channels();
		}
		setValid(true);

		if(!image && loadInfoOnline())
		{
			loadImageOnline();
		}
	}
	emit dataRecived();
}

bool LocalItem::readMpegImage()
{
	QByteArray byteArray = url().toLocal8Bit();
	if(byteArray.startsWith(FILE_MARCO))
	{
		byteArray.remove(0,sizeof(FILE_MARCO)-1);
	}
	MPEG::File file(byteArray.constData(),true);
	
	ID3v2::Tag *id3v2tag = file.ID3v2Tag();

    if(id3v2tag) 
	{
		ID3v2::FrameList::ConstIterator it = id3v2tag->frameList().begin();
		for(; it != id3v2tag->frameList().end(); it++)
		{
			if((*it)->frameID() == "APIC")
			{
			  ID3v2::AttachedPictureFrame *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame*>((*it));
			  m_image.loadFromData( (const unsigned char *) pic->picture().data(), pic->picture().size());
			  return true;
			}
		}
	}
	return false;
}

void LocalItem::loadImageOnline()
{
	Debug << "UNIMPLEMENTED";
}

bool LocalItem::isAsync() const
{
	return loadInfoOnline();
}

}; // namespace PlaylistItem
