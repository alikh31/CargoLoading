#ifndef PROVIDERTABLE_H
#define PROVIDERTABLE_H

#include <QTableView>
#include <QMenu>
#include <qaction.h>
#include "ProviderModel.h"
#include <QSortFilterProxyModel>


class CProviderTable : public QTableView
{
	Q_OBJECT

public:
	CProviderTable(QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CProviderTable();

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
	CProviderModel				*m_pModel;
	QSortFilterProxyModel	*m_pSortModel;
	QModelIndexList			 m_SelectedIndexes;



};

#endif // PROVIDERTABLE_H
