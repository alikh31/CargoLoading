#ifndef PALLETETABEL_H
#define PALLETETABEL_H

#include <QTableView>
#include <QMenu>
#include <qaction.h>
#include "PalleteModel.h"
#include <QSortFilterProxyModel>


class CPalleteTable : public QTableView
{
	Q_OBJECT

public:
	CPalleteTable(QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CPalleteTable();

	public slots:
		void				RequestForContextMenuEvent	( const QPoint& point);	
		void				AddNewRowEvent				( );	
		void				DeleteEvent			( );	
public:
	void				SetupActions				();
	QModelIndexList		getSelectedRows				();
	void				ReLoadModel					();
public:
	QMenu				*m_pMenu;
	QAction				*m_pAddNewRowAct;
	QAction				*m_pDeleteCurrentRowAct;
public:
	CGUIMainWindow* m_pMainWindow;
public:
	CPalleteModel				*m_pModel;
	QSortFilterProxyModel	*m_pSortModel;
	QModelIndexList			 m_SelectedIndexes;
};

#endif // CARRIERTABLE_H
