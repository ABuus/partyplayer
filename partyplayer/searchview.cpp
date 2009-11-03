#include "searchview.h"

SearchView::SearchView(QWidget *parent)
	: QListView(parent)
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setDragEnabled(true);
	setAcceptDrops(false);
}

SearchView::~SearchView()
{

}

