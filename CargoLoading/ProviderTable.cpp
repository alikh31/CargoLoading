

#pragma region "Includes"
#include "ProviderTable.h"
#include "GUIMainWindow.h"
#pragma endregion

#pragma region "Constructor"
CProviderTable::CProviderTable(QWidget *parent,CGUIMainWindow* pMainWindow)
: QTableView(parent)
{
	m_pMainWindow=pMainWindow;
	setContextMenuPolicy(Qt::CustomContextMenu);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	m_pModel=new CProviderModel(pMainWindow);
	m_pSortModel=new QSortFilterProxyModel();
	setModel(m_pModel);
	bool bres=connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ),this, SLOT( RequestForContextMenuEvent( const QPoint& ) ) );
	bres;
	setSelectionBehavior(QAbstractItemView::SelectRows);
	SetupActions();
}
#pragma endregion

#pragma region "DeConstructor"
CProviderTable::~CProviderTable()
{

}
#pragma endregion

#pragma region "void CImageTable::SetupActions()"
void CProviderTable::SetupActions()
{
	m_pMenu=new QMenu();

	m_pMenu->addAction(QString("copy"));//"copy"
	m_pMenu->addAction(QString("paste"));//"paste"
	m_pAddNewRowAct = m_pMenu->addAction(QString("Add New Row"));//"Add New Row"
	connect(m_pAddNewRowAct , SIGNAL(triggered()),this, SLOT(AddNewRowEvent()));

	m_pDeleteCurrentRowAct=m_pMenu->addAction(QString("Delete Selected"));//"Delete Selected"
	connect(m_pDeleteCurrentRowAct, SIGNAL(triggered()),this, SLOT(DeleteEvent()));
}
#pragma endregion

#pragma region "void CProviderTable::AddNewRowEvent( )"
void CProviderTable::AddNewRowEvent( )
{
	//m_pMainWindow->m_pProject->AddNewProvider();
	ReLoadModel();

}
#pragma endregion

#pragma region "void CProviderTable::DeleteEvent( )"
void CProviderTable::DeleteEvent( )
{

}
#pragma endregion

#pragma region "void CProviderTable::RequestForContextMenuEvent	( const QPoint& point)"
void CProviderTable::RequestForContextMenuEvent	( const QPoint& point)
{
	int test=10;
	QPoint pnt=mapToGlobal(point);
	pnt.setX(pnt.x()+10);
	pnt.setY(pnt.y()+10);
	m_SelectedIndexes=getSelectedRows();//selectionModel();
	if(m_SelectedIndexes.count()>0)
		m_pMenu->exec(pnt);
}
#pragma endregion

#pragma region "QModelIndexList CProviderTable::getSelectedRows()"
QModelIndexList CProviderTable::getSelectedRows()
{
	QModelIndexList lstIndex ;
	QItemSelection ranges = selectionModel()->selection();
	for (int i = 0; i < ranges.count(); ++i)
	{
		QModelIndex parent = ranges.at(i).parent();
		int right = ranges.at(i).model()->columnCount(parent) - 1;
		if (ranges.at(i).left() == 0 && ranges.at(i).right() == right)
			for (int r = ranges.at(i).top(); r <= ranges.at(i).bottom(); ++r)
				lstIndex.append(ranges.at(i).model()->index(r, 0, parent));
	}
	return lstIndex;
}
#pragma endregion

#pragma region "void CProviderTable::ReLoadModel()"
void CProviderTable::ReLoadModel()
{
	return m_pModel->RefreshList();
	setModel(NULL);
	delete m_pModel;
	m_pModel=NULL;
	m_pModel=new CProviderModel(m_pMainWindow);
	setModel(m_pModel);
}
#pragma endregion