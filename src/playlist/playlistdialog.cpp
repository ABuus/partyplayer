#include "playlistdialog.h"

using namespace Playlist;

PlaylistDialog::PlaylistDialog(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent,f)
{
	setupUi(this);
	connect(this,SIGNAL(accepted()),this,SLOT(savePlaylist()));
	connect(nameEdit,SIGNAL(textChanged(const QString &)),this,SLOT(validatePlaylistName(const QString &)));
	validatePlaylistName("");
	m_playlistModel = new SavedPlaylistModel(this);
	listView->setModel(m_playlistModel);
	listView->setRootIndex(m_playlistModel->index(SavedPlaylistModel::playlistPath()));
	connect(listView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(listViewItemClicked(const QModelIndex &)));
}

PlaylistDialog::~PlaylistDialog()
{

}

void PlaylistDialog::savePlaylist()
{
	if(m_urls.isEmpty())
	{
		QMessageBox::critical(this,tr("Playlist error"),tr("No items in playlist"));
		return;
	}
	const QString name = nameEdit->text();
	if(playlistExists(name))
	{
		if(!warnOverride())
			return;
	}
	createPlaylistPath();
	QFile file(absolutePlaylistPath(name));
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(this,tr("File error"), tr("Can not write to playlist"));
		return;
	}
	for( int i = 0; i < m_urls.count(); i++)
	{
		file.write(m_urls.at(i).toString().toLatin1() + "\n"); 
	}
	file.close();
}

void PlaylistDialog::validatePlaylistName(const QString &name)
{
	bool valid = false;
	if(!name.isEmpty())
	{
		valid = true;
	}
	okButton->setEnabled(valid);	
}

bool PlaylistDialog::playlistExists(const QString &name)
{
	QFileInfo playlistInfo( absolutePlaylistPath(name) );
	return playlistInfo.exists();
}

void PlaylistDialog::setUrls(const QList<QUrl> urls)
{
	m_urls = urls;
}

QString PlaylistDialog::absolutePlaylistPath(const QString &name)
{
	return QString(SavedPlaylistModel::playlistPath() + name);
}

bool PlaylistDialog::warnOverride()
{
	QString message(tr("A playlist with the name %1 allready exists, do you want to override it"));
	QMessageBox messageBox(QMessageBox::Warning,
		tr("Playlist exists"),
		message.arg(nameEdit->text()),
		QMessageBox::Ok | QMessageBox::Cancel,
		this);
	if(messageBox.exec() == QMessageBox::Ok)
		return true;
	else
		return false;
}

bool PlaylistDialog::createPlaylistPath()
{
	QDir dir;
	return dir.mkpath(SavedPlaylistModel::playlistPath());
}


void PlaylistDialog::listViewItemClicked(const QModelIndex &index)
{
	if(!index.isValid())
		return;
	QString text = m_playlistModel->data(index).toString();
	nameEdit->setText(text);
}