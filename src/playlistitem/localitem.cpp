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

namespace PlaylistItem {

LocalItem::LocalItem(const QString &url,QObject *parent)
	: PlaylistItem(url,parent)
{
	init();
}

LocalItem::LocalItem(const QUrl &url,QObject *parent)
	:PlaylistItem(url,parent)
{
	init();
}

LocalItem::~LocalItem()
{
}

void LocalItem::init()
{
	QByteArray byteArray = url().toLocal8Bit();
	if(byteArray.startsWith(FILE_MARCO))
	{
		byteArray.remove(0,sizeof(FILE_MARCO)-1);
	}

	TagLib::FileName tFileName = byteArray.constData();
	TagLib::FileRef tFileRef(tFileName,true);
	TagLib::File *tFile = tFileRef.file();

	if(!tFileRef.isNull() && tFileRef.tag())
	{
		/* * indicate at runtime */
		if(typeid(*tFile) == typeid(TagLib::MPEG::File) )
		{
			readMpegImage();
		}

		TagLib::Tag *tTag = tFileRef.tag();
		m_artist = QString::fromUtf8(tTag->artist().toCString(true));		
		m_title = QString::fromUtf8(tTag->title().toCString(true));		
		m_album = QString::fromUtf8(tTag->album().toCString(true));		
		m_comment = QString::fromUtf8(tTag->comment().toCString(true));		
		m_genre = QString::fromUtf8(tTag->genre().toCString(true));
		m_year = tTag->year();
		m_track = tTag->track();
		
		if(tFileRef.audioProperties())
		{
			TagLib::AudioProperties *tAudioProperties = tFileRef.audioProperties();
			m_length = tAudioProperties->length();
			m_bitrate = tAudioProperties->bitrate();
			m_samplerate = tAudioProperties->sampleRate();
			m_channels = tAudioProperties->channels();
		}
		setValid(true);
	}
	delete tFile;
}

void LocalItem::readMpegImage()
{
	Debug << "MPEG FILE";
	QByteArray byteArray = url().toLocal8Bit();
	if(byteArray.startsWith(FILE_MARCO))
	{
		byteArray.remove(0,sizeof(FILE_MARCO)-1);
	}
	TagLib::MPEG::File tFile(byteArray.constData());
	if(!tFile.isValid() || !tFile.isOpen())
	{
		Debug << "invalid mpeg";
		return;
	}
	TagLib::ID3v2::Tag *tag = tFile.ID3v2Tag();
	if(tag->isEmpty())
	{
		Debug << "empty tag";
		return;
	}
	TagLib::ID3v2::FrameList frameList = tag->frameListMap()["APIC"];
	if(frameList.isEmpty())
		return;
	TagLib::ID3v2::AttachedPictureFrame *picFrame;
	for(TagLib::ID3v2::FrameList::ConstIterator it = frameList.begin(); it != frameList.end(); ++it)
	{
	  picFrame = (TagLib::ID3v2::AttachedPictureFrame *)(*it);
	  if(picFrame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
	  {
	    TagLib::ID3v2::AttachedPictureFrame *attachedPictureFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);
	    QByteArray imageData;
	    imageData.fromBase64(attachedPictureFrame->picture().data());
	    m_image.loadFromData(imageData.fromBase64(imageData));
	    return;
	  }
	}
}

}; // namespace PlaylistItem
