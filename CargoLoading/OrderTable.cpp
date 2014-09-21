#include "OrderTable.h"
#include "GUIMainWindow.h"
#include "OrderWidget.h"

COrderTable::COrderTable(QWidget *parent,CGUIMainWindow* pMainWindow)
: QTableView(parent)
{
	m_pMainWindow=pMainWindow;
	setContextMenuPolicy(Qt::CustomContextMenu);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	m_pModel=new COrderModel(pMainWindow);
	m_pSortModel=new QSortFilterProxyModel();
	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
	bool bres=connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ),this, SLOT( RequestForContextMenuEvent( const QPoint& ) ) );
	bres;
	setSelectionBehavior(QAbstractItemView::SelectRows);
	SetupActions();
}

COrderTable::~COrderTable()
{

}

void COrderTable::SetupActions()
{
	m_pMenu=new QMenu();

	m_pMenu->addAction(QString("copy"));//"copy"
	m_pMenu->addAction(QString("paste"));//"paste"
	m_pAddNewRowAct = m_pMenu->addAction(QString("Add New Row"));//"Add New Row"
	connect(m_pAddNewRowAct , SIGNAL(triggered()),this, SLOT(AddNewRowEvent()));
	
	m_pDeleteCurrentRowAct=m_pMenu->addAction(QString("Delete Selected"));//"Delete Selected"
	connect(m_pDeleteCurrentRowAct, SIGNAL(triggered()),this, SLOT(DeleteEvent()));
}

void COrderTable::AddNewRowEvent( )
{
	//m_pMainWindow->m_pProject->AddNewOrder();

	//m_pMainWindow->m_pCurrentOrderListTab->OnAddNewRow(true);

	ReLoadModel();

}

void COrderTable::DeleteEvent( )
{
	int row;
	QList<int> deleteList;
	for (int i = 0; i < m_SelectedIndexes.count(); ++i)
	{
		QModelIndex Sindex = m_SelectedIndexes.at(i);
		QModelIndex index = m_pSortModel->mapToSource(Sindex);
		row=index.row();
		deleteList.append(row);
	}

	qSort(deleteList.begin(), deleteList.end());

	while (!deleteList.isEmpty())
	{
		row = deleteList.takeLast();
		m_pMainWindow->m_pProject->m_OrderList.removeAt(row);
	}
	ReLoadModel();
}

void COrderTable::RequestForContextMenuEvent	( const QPoint& point)
{
	int test=10;
	QPoint pnt=mapToGlobal(point);
	pnt.setX(pnt.x()+10);
	pnt.setY(pnt.y()+10);
	m_SelectedIndexes=getSelectedRows();//selectionModel();
	if(m_SelectedIndexes.count()>0)
		m_pMenu->exec(pnt);
}

QModelIndexList COrderTable::getSelectedRows()
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

void COrderTable::ReLoadModel()
{
	return m_pModel->RefreshList();
	setModel(NULL);
	delete m_pModel;
	m_pModel=NULL;
	m_pModel=new COrderModel(m_pMainWindow);
	setModel(m_pSortModel);
}