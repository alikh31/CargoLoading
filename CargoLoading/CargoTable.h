#ifndef CARGOTABLE_H
#define CARGOTABLE_H

#include <QTableView>
#include <QMenu>
#include <qaction.h>
#include "CargoModel.h"
#include <QSortFilterProxyModel>
#include <qalgorithms.h>

class CCargoTable : public QTableView
{
	Q_OBJECT

public:
	CCargoTable(QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CCargoTable();

public slots:
	void				RequestForContextMenuEvent	( const QPoint& point);	
	void				AddNewRowEvent				( );	
	void				DeleteEvent			( );	
public:
	void				SetupActions				();
	QModelIndexList		getSelectedRows				();
	void				ReLoadModel					();
	void				Update						();
public:
	QMenu				*m_pMenu;
	QAction				*m_pAddNewRowAct;
	QAction				*m_pDeleteCurrentRowAct;
public:
	CGUIMainWindow* m_pMainWindow;
public:
	CCargoModel				*m_pModel;
	QSortFilterProxyModel	*m_pSortModel;
	QModelIndexList			 m_SelectedIndexes;



};

#endif // CARGOTABLE_H
