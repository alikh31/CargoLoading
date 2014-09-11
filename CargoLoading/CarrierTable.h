#ifndef CARRIERTABEL_H
#define CARRIERTABEL_H

#include <QTableView>
#include <QMenu>
#include <qaction.h>
#include "CarrierModel.h"
#include <QSortFilterProxyModel>
#include <QMouseEvent>


class CCarrierTable : public QTableView
{
	Q_OBJECT

public:
	CCarrierTable(QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CCarrierTable();

	public slots:
		void				RequestForContextMenuEvent	( const QPoint& point);	
		void				mouseDoubleClickEvent		( QMouseEvent * event );
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
	CCarrierModel				*m_pModel;
	QSortFilterProxyModel	*m_pSortModel;
	QModelIndexList			 m_SelectedIndexes;
};

#endif // CARRIERTABLE_H
