#ifndef ORDERTABEL_H
#define ORDERTABEL_H

#include <QTableView>
#include <QMenu>
#include <qaction.h>
#include "OrderModel.h"
#include <QSortFilterProxyModel>


class COrderTable : public QTableView
{
	Q_OBJECT

public:
	COrderTable(QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~COrderTable();

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
	COrderModel				*m_pModel;
	QSortFilterProxyModel	*m_pSortModel;
	QModelIndexList			 m_SelectedIndexes;
};

#endif // CARRIERTABLE_H
