#ifndef CPlanTree_H
#define CPlanTree_H

#include <QTreeWidget.h>
#include "QList"
#include "Project.h"
#include "GUIMainWindow.h"
#include <qaction.h>
#include <QMenu>


//class CProject;
class CProjectionArray;
//class CGUIMainWidow;
class CPlanTree : public QTreeWidget
{
	Q_OBJECT

public:
	CPlanTree(QWidget *parent,CGUIMainWindow* pMainWindow,CProject *Project);
	~CPlanTree();
public:
	void Initialize();
	void CreateBody();
	void CreateActions();
public:
	//TODO : Tree Items :
	QTreeWidgetItem* m_pPathItem;
	QTreeWidgetItem* m_pNodeItem;
public:
	QMenu*   pProjectMenu;
	QMenu*   pProjectExpnadedMenu;
	QMenu*   pFrameMenu;
	QAction* m_pCollapseAct;
	QAction* m_pExpandAct;

	QAction* m_pCollapseProjectionAct;
	QAction* m_pExpandProjectionAct;

	QAction* m_pChangeArangeModeAct;
	QAction* m_pAddProjectionArrayAct;
	QAction* m_pProjectSettingAct;

	QMenu* pFrameArrayMenu;
	QAction* m_pCollapseFmAct;
	QAction* m_pExpandFmAct;
	QAction* m_pActivateFmAct;
	QAction* m_pDeActivateFmAct;
	QAction* m_pAddImageFmAct;
	QAction* m_pAddImageFromFolderFmAct;
	QAction* m_pAutoMeasureFmAct;
	QAction* m_pDeleteFmAct;
	QAction* m_pImportFmAct;
	QAction* m_pExportFmAct;
	QAction* m_pImportParameterFmAct;
	QAction* m_pImportImagePointFmAct;
	QAction* m_pImportExteriorFmAct;
	QAction* m_pExportParameterFmAct;
	QAction* m_pExportImagePointFmAct;
	QAction* m_pExportExteriorFmAct;

//Panoramic Action 
	QMenu*   pPanoramicMenu;
	QMenu*   pPanoramicArrayMenu;
	QAction* m_pCollapsePanAct;
	QAction* m_pExpandPanAct;
	QAction* m_pActivatePanAct;
	QAction* m_pDeActivatePanAct;
	QAction* m_pAddImagePanAct;
	QAction* m_pAddImageFromFolderPanAct;
	QAction* m_pAutoMeasurePanAct;
	QAction* m_pDeletePanAct;
	QAction* m_pImportPanAct;
	QAction* m_pExportPanAct;
	QAction* m_pImportParameterPanAct;
	QAction* m_pImportImagePointPanAct;
	QAction* m_pImportExteriorPanAct;
	QAction* m_pExportParameterPanAct;
	QAction* m_pExportImagePointPanAct;
	QAction* m_pExportExteriorPanAct;
//End Panoramic Action 

	QMenu* pImageMenu;
	QAction* m_pCollapseImAct;
	QAction* m_pExpandImAct;
	QAction* m_pActivateImAct;
	QAction* m_pDeActivateImAct;
	QAction* m_pLinkToFileImAct;
	QAction* m_pDeleteImAct;

	QMenu* pScaleBarMenu;
	QAction* m_pCollapseSBAct;
	QAction* m_pExpandSBAct;
	QAction* m_pAddScaleBarSBAct;
	QAction* m_pImportParameterSBAct;
	
	QMenu* pScaleBarItemMenu;
	QAction* m_pCollapseSIAct;
	QAction* m_pExpandSIAct;
	QAction* m_pActivateSIAct;
	QAction* m_pDeActivateSIAct;
	QAction* m_pDeleteSIAct;
	QAction* m_pExportParameterSIAct;



public slots:
	void currentItemChangedEvent ( QTreeWidgetItem * current, QTreeWidgetItem * previous );
	void RequestForContextMenuEvent(const QPoint & pos );
	void itemClickedEvent ( QTreeWidgetItem * item, int column ) ;
	void itemDoubleClickedEvent( QTreeWidgetItem * item, int column ) ;
	void itemChangedEvent ( QTreeWidgetItem * item, int column );
public:
	void OpenEditor(QTreeWidgetItem * item, int column );
	__BoxRow* GetBox(int id);
	Path* GetPath(int id);

public slots:
	void OnCollapse();
	void OnExpand();
	void OnCollapseProj();
	void OnExpandProj();
	void OnChangeArangeMode();
	void OnAddProjectionArray();
	void OnProjectSetting();
public:
	CGUIMainWindow*		m_pMainWindow;
	QTreeWidgetItem *	m_pEditingItem;
	int					m_EditingColumnNo;
	bool				m_bIsEditingStarted;
	CProject*			m_pProject;
	CProjectionArray*	m_currentContextProjectionArray;
	bool				m_bProjectCollapse;
	Path*				m_pSelectedPath;

private:
	
};

#endif // CPlanTree_H

