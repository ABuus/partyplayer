/*
** This file is part of the Party Player application.
** Copyright (C) 2009  Anders Buus
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include "playlistitem_global.h"
#include <QObject>
#include <QPixmap>
#include <QUrl>
#include <QPersistentModelIndex>
#include "../debug.h"

namespace PlaylistItem {

class PLAYLISTITEM_EXPORT PlaylistItem : public QObject
{
	Q_OBJECT
public:
	enum Type {
		NoType = 0,
		Local,
		Youtube
	};
	PlaylistItem(const QString &url,QObject *parent = 0);
	PlaylistItem(const QUrl &url,QObject *parent = 0);
	~PlaylistItem();
	QString url() const;
	bool loadInfoOnline() const { return m_loadInfoOnline; };
	bool isValid() const { return m_isValid; };
	static QString lengthAsPrittyPrint(uint length);
	virtual QString artist() const { return 0; };
	virtual QString title() const { return 0; };
	virtual QString album() const { return 0; };
	virtual QString comment() const { return 0; };
	virtual QString genre() const { return 0; };
	virtual uint track() const { return 0; };
	/* in seconds */
	virtual uint length() const { return 0; };
	virtual uint year() const { return 0; };
	virtual uint bitrate() const { return 0; };
	virtual uint channels() const { return 0; };
	virtual uint samplerate() const { return 0; };
	virtual QPixmap image() const { return 0; };
	virtual int type() const = 0;
	virtual bool isAsync() const = 0;
	virtual void fetchData() = 0;
	void setIndex(QPersistentModelIndex index) { m_index = index; };
	QPersistentModelIndex index() { return m_index; };
private:
	QString m_url;
	bool m_loadInfoOnline;
	bool m_isValid;
	void init();
	QPersistentModelIndex m_index;
protected:
	void setValid(bool valid) { m_isValid = valid; };
signals:
	void dataRecived();
};

}; // namespace PlaylistItem

#endif // PLAYLISTITEM_H
