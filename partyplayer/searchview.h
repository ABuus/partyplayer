#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QListView>
#include <QMouseEvent>

class SearchView : public QListView
{
	Q_OBJECT

public:
	SearchView(QWidget *parent);
	~SearchView();

private:
protected:
};

#endif // SEARCHVIEW_H
