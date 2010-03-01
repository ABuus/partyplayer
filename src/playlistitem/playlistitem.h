#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include "playlistitem_global.h"
#include <QPixmap>
#include <QUrl>

namespace PlaylistItem {

class PLAYLISTITEM_EXPORT PlaylistItem : public QObject
{
	Q_OBJECT
public:
	PlaylistItem(const QString &url,QObject *parent = 0);
	PlaylistItem(const QUrl &url,QObject *parent = 0);
	~PlaylistItem();
	QString url() const;
	bool loadInfoOnline() const;
	bool isValid() { return m_isValid; };
	static QString lengthAsPrittyPrint(uint length);
	virtual QString artist() const = 0;
	virtual QString title() const = 0;
	virtual QString album() const = 0;
	virtual QString comment() const = 0;
	virtual QString genre() const = 0;
	virtual uint track() const = 0;
	/* in seconds */
	virtual uint length() const = 0;
	virtual uint year() const = 0;
	virtual uint bitrate() const = 0;
	virtual uint channels() const = 0;
	virtual uint samplerate() const = 0;
	virtual QPixmap image() const = 0;
private:
	const QString m_url;
	bool m_loadInfoOnline;
	bool m_isValid;
	void init();
protected:
	void setValid(bool valid) { m_isValid = valid; };
};

}; // namespace PlaylistItem

#endif // PLAYLISTITEM_H
