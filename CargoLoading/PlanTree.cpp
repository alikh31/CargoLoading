#pragma region "includes"
#include "PlanTree.h"
//#include "PathOptimizer.h"
#include "QTreeWidgetItem"
#include "QStringList"
#include "QAction"
#include "QMessageBox"
#include <qdatetime.h>
#include <QFileDialog>
#include <QIcon>
#include "QSize"
#pragma endregion

#pragma region "CPlanTree::CPlanTree(QWidget *parent,CProject *Project)"
CPlanTree::CPlanTree(QWidget *parent,CGUIMainWindow* pMainWindow,CProject *Project)
:QTreeWidget(parent)
{
	m_pMainWindow=pMainWindow;
	m_pProject = m_pMainWindow->m_pProject;
	Initialize();
	CreateBody();
	CreateActions();
	
}
#pragma endregion

#pragma region "CPlanTree::~CPlanTree()"
CPlanTree::~CPlanTree()
{

}
#pragma endregion

#pragma region "void CPlanTree::OpenEditor()"
void CPlanTree::OpenEditor(QTreeWidgetItem * item, int column )
{
	openPersistentEditor(item,column);
	m_bIsEditingStarted= true;
	m_EditingColumnNo=column;
	m_pEditingItem=item;
}
#pragma endregion

/*╔════User Events═════════╗*/
#pragma region "void CPlanTree::currentItemChangedEvent( QTreeWidgetItem * current, QTreeWidgetItem * previous )"
void CPlanTree::currentItemChangedEvent( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
	

}
#pragma endregion

#pragma region "void CPlanTree::RequestForContextMenuEvent (const  QPoint & pos )"
void CPlanTree::RequestForContextMenuEvent (const  QPoint & pos )
{
	QTreeWidgetItem * item = 0 ;
	item = itemAt(pos ) ;
	if(!item)
	{
		return;
	}
	if(item->parent())
	{
		m_pSelectedPath = GetPath(item->parent()->text(0).toInt());
		pProjectMenu->exec(mapToGlobal(pos));
		return;
	}
	if(item)
	{
		m_pSelectedPath = GetPath(item->text(0).toInt());
		pProjectMenu->exec(mapToGlobal(pos));
		return;
	}
}
#pragma endregion

#pragma region "void CPlanTree::itemDoubleClickedEvent ( QTreeWidgetItem * item, int column )"
void CPlanTree::itemDoubleClickedEvent ( QTreeWidgetItem * item, int column ) 
{
	
}
#pragma endregion

#pragma region "void CPlanTree::itemClickedEvent( QTreeWidgetItem * item, int column ) "
void CPlanTree::itemClickedEvent( QTreeWidgetItem * item, int column ) 
{
	if(item->parent()){
        return;
    }
		//m_pMainWindow->m_pMainWidget->path = GetPath(item->text(0).toInt());// ->Update(GetPath(item->parent()->text(0).toInt()),item->text(0).toInt());
	else{
		m_pMainWindow->m_pGLWidget->path = GetPath(item->text(0).toInt());
        m_pMainWindow->m_pGLWidget->update();
    }
}
#pragma endregion

#pragma region "void CPlanTree::itemChangedEvent ( QTreeWidgetItem * item, int column )"
void CPlanTree::itemChangedEvent ( QTreeWidgetItem * item, int column )
{
	//close the editor
	closePersistentEditor(item,column);
	m_bIsEditingStarted = false;
	m_EditingColumnNo=-1;
	m_pEditingItem=NULL;
}
#pragma endregion


#pragma region "void CPlanTree::Initialize()"
void CPlanTree::Initialize()
{
	QStringList list;
	list<<QString("Property")<<QString("Value");//"property", value
	setColumnCount(2);
	setHeaderLabels(list);list.clear();
	setColumnWidth(0,200);
	setColumnWidth(1,50);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ),this, SLOT( RequestForContextMenuEvent( const QPoint& ) ) );
	connect( this, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int)),this, SLOT( itemDoubleClickedEvent( QTreeWidgetItem *, int) ) );
	connect( this, SIGNAL( itemClicked( QTreeWidgetItem *, int)),this, SLOT( itemClickedEvent( QTreeWidgetItem *, int) ) );
	connect( this, SIGNAL( itemChanged( QTreeWidgetItem *, int)),this, SLOT( itemChangedEvent ( QTreeWidgetItem *, int) ) );
	connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this,SLOT(currentItemChangedEvent(QTreeWidgetItem *, QTreeWidgetItem *)));
}
#pragma endregion 

#pragma region "void CPlanTree::CreateBody()"
void CPlanTree::CreateBody()
{
	clear();
	foreach(Path* path,m_pProject->m_AllPathes)
	{
		QList<QString> list;
		list<<QString("%1").arg(path->id)<<QString("Carrier type : %1 ,Date : %2 to %3 , Cost : %4").arg(path->m_Carrier->name).arg(path->m_minTime).arg(path->m_maxTime).arg(path->m_Cost);//"Carrier type : %1 ,Date : %2 to %3 , Cost : %4"
		m_pPathItem=new QTreeWidgetItem(this,list);

		//QIcon icon(QPixmap("Resources/project.png"));
		//m_pProjectItem->setIcon(0,icon);

		m_pPathItem->setSizeHint(0,QSize(20,20));


		for(int j=0;j<path->m_Node.count();j++)
		{
			Node * node = path->m_Node.at(j);
			__BoxRow *boxRow = GetBox(node->m_Order->BoxID);
			list.clear();

			list<<QString("%1").arg(node->id)<<QString("Node type : %1 ,Provider: %2,Distance from the last Node: %3 , Number: %4").arg(boxRow->Desc).arg(node->m_Order->pProvider->name).arg(node->m_Dist).arg(node->m_Order->count);//"Node type : %1 ,Provider: %2,Distance from the last Node: %3 , Number: %4"

			m_pNodeItem=new QTreeWidgetItem(m_pPathItem,list);

			//QIcon icon(QPixmap("Resources/project.png"));
			//m_pProjectItem->setIcon(0,icon);

			m_pPathItem->setSizeHint(0,QSize(20,20));


		}	
		expandItem(m_pPathItem);

	}


	//expandItem(m_pProcessingResultItem);

}
#pragma endregion

#pragma region "void CPlanTree::CreateActions()"
void CPlanTree::CreateActions()
{
	m_pCollapseAct=new QAction("Collapse",this);
	connect(m_pCollapseAct, SIGNAL(triggered()), this, SLOT(OnCollapse()));

	m_pExpandAct=new QAction("Expand",this);
	connect(m_pExpandAct, SIGNAL(triggered()), this, SLOT(OnExpand()));

	m_pCollapseProjectionAct=new QAction("Collapse",this);
	connect(m_pCollapseProjectionAct, SIGNAL(triggered()), this, SLOT(OnCollapseProj()));

	m_pExpandProjectionAct=new QAction("Expand",this);
	connect(m_pExpandProjectionAct, SIGNAL(triggered()), this, SLOT(OnExpandProj()));

	m_pChangeArangeModeAct=new QAction("Change Arange Mode",this);
	m_pChangeArangeModeAct->setCheckable(true);
	connect(m_pChangeArangeModeAct, SIGNAL(triggered()), this, SLOT(OnChangeArangeMode()));

	m_pAddProjectionArrayAct=new QAction("Add Projection Array",this);
	connect(m_pAddProjectionArrayAct, SIGNAL(triggered()), this, SLOT(OnAddProjectionArray()));

	m_pProjectSettingAct=new QAction("Project Setting...",this);
	connect(m_pProjectSettingAct, SIGNAL(triggered()), this, SLOT(OnProjectSetting()));

	pProjectMenu=new QMenu(this);
	pProjectMenu->addAction(m_pCollapseAct);
	pProjectMenu->addSeparator();
	pProjectMenu->addAction(m_pChangeArangeModeAct);
	//pProjectMenu->addSeparator();
	//pProjectMenu->addAction(m_pProjectSettingAct);

	pProjectExpnadedMenu=new QMenu(this);
	pProjectExpnadedMenu->addAction(m_pExpandAct);
	pProjectExpnadedMenu->addSeparator();
	pProjectExpnadedMenu->addAction(m_pChangeArangeModeAct);
	pProjectExpnadedMenu->addSeparator();
	pProjectExpnadedMenu->addAction(m_pProjectSettingAct);

	pFrameMenu=new QMenu(this);
	pFrameArrayMenu=new QMenu(this);

	m_pCollapseFmAct=new QAction("Collapse",this);
	connect(m_pCollapseFmAct, SIGNAL(triggered()), this, SLOT(OnCollapseFm()));
	m_pExpandFmAct=new QAction("Expand",this);
	connect(m_pExpandFmAct, SIGNAL(triggered()), this, SLOT(OnExpandFm()));
	m_pActivateFmAct=new QAction("Activate",this);
	connect(m_pActivateFmAct, SIGNAL(triggered()), this, SLOT(OnActivateFm()));	
	m_pDeActivateFmAct=new QAction("Deactivate Frame Array",this);
	connect(m_pDeActivateFmAct, SIGNAL(triggered()), this, SLOT(OnDeActivateFm()));	
	m_pAddImageFmAct=new QAction("Add Image(s)(TODO:MAX 1024 Files) ",this);
	connect(m_pAddImageFmAct, SIGNAL(triggered()), this, SLOT(OnAddImageFm()));	
	m_pAddImageFromFolderFmAct=new QAction("Add Images From Folder ",this);
	connect(m_pAddImageFromFolderFmAct, SIGNAL(triggered()), this, SLOT(OnAddImageFromFolderFm()));	
	m_pAutoMeasureFmAct=new QAction("Auto Measure",this);
	connect(m_pAutoMeasureFmAct, SIGNAL(triggered()), this, SLOT(OnAutoMeasureFm()));	
	m_pDeleteFmAct=new QAction("Delete Frame Array",this);
	connect(m_pDeleteFmAct, SIGNAL(triggered()), this, SLOT(OnDeleteFm()));	
	m_pImportFmAct=new QAction("Import",this);
	m_pExportFmAct=new QAction("Export",this);
	m_pImportParameterFmAct=new QAction("Parameters",this);
	connect(m_pImportParameterFmAct, SIGNAL(triggered()), this, SLOT(OnImportParameterFm()));	
	m_pImportImagePointFmAct=new QAction("Image Points",this);
	connect(m_pImportImagePointFmAct, SIGNAL(triggered()), this, SLOT(OnImportImagePointFm()));	
	m_pImportExteriorFmAct=new QAction("Exterior Orientation Parameters",this);
	connect(m_pImportExteriorFmAct, SIGNAL(triggered()), this, SLOT(OnImportExteriorFm()));	
	m_pExportParameterFmAct=new QAction("Parameters",this);
	connect(m_pExportParameterFmAct, SIGNAL(triggered()), this, SLOT(OnExportParameterFm()));	
	m_pExportImagePointFmAct=new QAction("Image Points",this);
	connect(m_pExportImagePointFmAct, SIGNAL(triggered()), this, SLOT(OnImportImagePointFm()));	
	m_pExportExteriorFmAct=new QAction("Exterior Orientation Parameters",this);
	connect(m_pExportExteriorFmAct, SIGNAL(triggered()), this, SLOT(OnImportExteriorFm()));	

	pPanoramicMenu=new QMenu(this);
	pPanoramicArrayMenu=new QMenu(this);

	m_pCollapsePanAct=new QAction("Collapse",this);
	connect(m_pCollapsePanAct, SIGNAL(triggered()), this, SLOT(OnCollapsePan()));
	m_pExpandPanAct=new QAction("Expand",this);
	connect(m_pExpandPanAct, SIGNAL(triggered()), this, SLOT(OnExpandPan()));
	m_pActivatePanAct=new QAction("Activate",this);
	connect(m_pActivatePanAct, SIGNAL(triggered()), this, SLOT(OnActivatePan()));	
	m_pDeActivatePanAct=new QAction("Deactivate Panoramic Array",this);
	connect(m_pDeActivatePanAct, SIGNAL(triggered()), this, SLOT(OnDeActivatePan()));	
	m_pAddImagePanAct=new QAction("Add Image(s)(TODO:MAX 1024 Files) ",this);
	connect(m_pAddImagePanAct, SIGNAL(triggered()), this, SLOT(OnAddImagePan()));	
	m_pAddImageFromFolderPanAct=new QAction("Add Images From Folder ",this);
	connect(m_pAddImageFromFolderPanAct, SIGNAL(triggered()), this, SLOT(OnAddImageFromFolderPan()));	
	m_pAutoMeasurePanAct=new QAction("Auto Measure",this);
	connect(m_pAutoMeasurePanAct, SIGNAL(triggered()), this, SLOT(OnAutoMeasurePan()));	
	m_pDeletePanAct=new QAction("Delete Panoramic Array",this);
	connect(m_pDeletePanAct, SIGNAL(triggered()), this, SLOT(OnDeletePan()));	
	m_pImportPanAct=new QAction("Import",this);
	m_pExportPanAct=new QAction("Export",this);
	m_pImportParameterPanAct=new QAction("Parameters",this);
	connect(m_pImportParameterPanAct, SIGNAL(triggered()), this, SLOT(OnImportParameterPan()));	
	m_pImportImagePointPanAct=new QAction("Image Points",this);
	connect(m_pImportImagePointPanAct, SIGNAL(triggered()), this, SLOT(OnImportImagePointPan()));	
	//m_pImportExteriorFmAct=new QAction("Exterior Orientation Parameters",this);
	//connect(m_pImportExteriorFmAct, SIGNAL(triggered()), this, SLOT(OnImportExteriorFm()));	
	m_pExportParameterPanAct=new QAction("Parameters",this);
	connect(m_pExportParameterPanAct, SIGNAL(triggered()), this, SLOT(OnExportParameterPan()));	
	m_pExportImagePointPanAct=new QAction("Image Points",this);
	connect(m_pExportImagePointPanAct, SIGNAL(triggered()), this, SLOT(OnImportImagePointPan()));	
	//m_pExportExteriorFmAct=new QAction("Exterior Orientation Parameters",this);
	//connect(m_pExportExteriorFmAct, SIGNAL(triggered()), this, SLOT(OnImportExteriorFm()));	



	pImageMenu=new QMenu(this);

	m_pCollapseImAct=new QAction("Collapse",this);
	connect(m_pCollapseImAct, SIGNAL(triggered()), this, SLOT(OnCollapseIm()));
	m_pExpandImAct=new QAction("Expand",this);
	connect(m_pExpandImAct, SIGNAL(triggered()), this, SLOT(OnExpandIm()));
	m_pActivateImAct=new QAction("Activate",this);
	connect(m_pActivateImAct, SIGNAL(triggered()), this, SLOT(OnActivateIm()));	
	m_pDeActivateImAct=new QAction("Deactivate",this);
	connect(m_pDeActivateImAct, SIGNAL(triggered()), this, SLOT(OnDeActivateIm()));	
	m_pLinkToFileImAct=new QAction("Link to a File",this);
	connect(m_pLinkToFileImAct, SIGNAL(triggered()), this, SLOT(OnLinkToFileIm()));	
	m_pDeleteImAct=new QAction("Delete",this);
	connect(m_pDeleteImAct, SIGNAL(triggered()), this, SLOT(OnDeleteIm()));	

	pScaleBarMenu=new QMenu(this);
	m_pCollapseSBAct=new QAction("Collapse",this);
	connect(m_pCollapseSBAct, SIGNAL(triggered()), this, SLOT(OnCollapseSB()));
	m_pExpandSBAct=new QAction("Expand",this);
	connect(m_pExpandSBAct, SIGNAL(triggered()), this, SLOT(OnExpandSB()));
	m_pAddScaleBarSBAct=new QAction("Add ScaleBar",this);
	connect(m_pAddScaleBarSBAct, SIGNAL(triggered()), this, SLOT(OnAddScaleBar()));	
	m_pImportParameterSBAct=new QAction("Scale Bar Parameters ...",this);
	connect(m_pImportParameterSBAct, SIGNAL(triggered()), this, SLOT(OnImportParamSB()));	


	pScaleBarItemMenu=new QMenu(this);
	m_pCollapseSIAct=new QAction("Collapse",this);
	connect(m_pCollapseSIAct, SIGNAL(triggered()), this, SLOT(OnCollapseSI()));
	m_pExpandSIAct=new QAction("Expand",this);
	connect(m_pExpandSIAct, SIGNAL(triggered()), this, SLOT(OnExpandSI()));
	m_pActivateSIAct=new QAction("Activate Scale Bar",this);
	connect(m_pActivateSIAct, SIGNAL(triggered()), this, SLOT(OnActivateSI()));	
	m_pDeActivateSIAct=new QAction("Deactivate Scale Bar",this);
	connect(m_pDeActivateSIAct, SIGNAL(triggered()), this, SLOT(OnDeActivateSI()));	
	m_pDeleteSIAct=new QAction("Delete Scale Bar",this);
	connect(m_pDeleteSIAct, SIGNAL(triggered()), this, SLOT(OnDeleteSI()));	

	m_pExportParameterSIAct=new QAction("Scale Bar Parameters ...",this);
	connect(m_pExportParameterSIAct, SIGNAL(triggered()), this, SLOT(OnExportParamSI()));	

	
	

	


}
#pragma endregion
#pragma endregion
/*╚════════════════════════╝*/

/*╔════Custom Slots ═══════╗*/
#pragma region "void CPlanTree::OnCollapse()"
void CPlanTree::OnCollapse()
{
}
#pragma endregion

#pragma region "void CPlanTree::OnExpand()"
void CPlanTree::OnExpand()
{
}
#pragma endregion

#pragma region "void CPlanTree::OnCollapseProj()"
void CPlanTree::OnCollapseProj()
{
}
#pragma endregion

#pragma region "void CPlanTree::OnExpandProj()"
void CPlanTree::OnExpandProj()
{
}
#pragma endregion

#pragma region "void CPlanTree::OnAddFrameArray()"
void CPlanTree::OnChangeArangeMode()
{
	/*if(m_pSelectedPath)
	{
		m_pProject->m_pCPathOptimizer->m_Carrier = m_pSelectedPath->m_Carrier;

		if(m_pChangeArangeModeAct->isChecked())
		{
			m_pProject->m_pCPathOptimizer->MaxOptimization(m_pSelectedPath);

		}
		else
		{
			m_pProject->m_pCPathOptimizer->replaceNode(m_pSelectedPath);

		}
		m_pMainWindow->m_pNetworkWidget->Update(m_pSelectedPath);
	}
*/
}
#pragma endregion

#pragma region "void CPlanTree::OnAddProjectionArray()"
void CPlanTree::OnAddProjectionArray()
{
}
#pragma endregion

#pragma region "void CPlanTree::OnProjectSetting()"
void CPlanTree::OnProjectSetting()
{

}
#pragma endregion
/*╚════════════════════════╝*/

#pragma region "__BoxRow* CPlanTree::GetBox(int id)" 
__BoxRow* CPlanTree::GetBox(int id)
{
	for(int i=0;i<m_pProject->m_CargoList.count();i++)
	{
		__BoxRow* box=m_pProject->m_CargoList.at(i);
		if(box->id==id)
			return box;
	}
	return NULL;
}
#pragma endregion

#pragma region "Path* CPlanTree::GetPath(int id)" 
Path* CPlanTree::GetPath(int id)
{
	for(int i=0;i<m_pProject->m_AllPathes.count();i++)
	{
		Path* path=m_pProject->m_AllPathes.at(i);
		if(path->id==id)
			return path;
	}
	return NULL;
}
#pragma endregion