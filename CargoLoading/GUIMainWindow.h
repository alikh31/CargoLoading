#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>
#include <QHBoxLayout>
//#include "ShortKeysDV.h"
#include "Project.h"
#include <QMenu>
#include <QAction>
//#include "NetworkWidget.h"
//#include "PlanTree.h"
#include "glwidget.h"


class CCargoWidget;
class CProviderWidget;
class CCarrierWidget;
class CPalleteWidget;
class COrderWidget;
class CPlanTree;
class CGUIMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	CGUIMainWindow(QWidget *parent = 0);
	~CGUIMainWindow();
public:
	QTabWidget					*center;
	CCargoWidget				*m_pCargoTab;
	CProviderWidget				*m_pProviderTab;
	CPalleteWidget				*m_pPalleteRelationTab;
	CCarrierWidget				*m_pCarriersTab;
	COrderWidget				*m_pCurrentOrderListTab;
	CPlanTree					*m_pPlanTree;
	QDockWidget					*m_pPlanWidget;
	QTableView					*m_pCurrentPlanTab;
    GLWidget                    *m_pMainWidget;
	//CShortKeysDV				*m_pShortkeyDV;
	//CNetworkWidget				*m_pNetworkWidget;
	
	bool m_fFirstTime;
public:
	void setupCentralWidget			();
	void setupDockWidget			();
	void setupMenuBar				();
	void UpdateAll					();
public:
	CProject * m_pProject;
	QMenu						*m_pViewMenu;
	QAction						*m_pNewProjAction;
	QAction						*m_pOpenProjAction;
	QAction						*m_pSaveProjAction;
	QAction						*m_pComputeAction;
	QAction						*m_pSequentialModeAction;
	QAction						*m_pOptimizedModeAction;
public slots:
	void NewProject					();
	void OpenProject				();
	void SaveProject				();
	void SaveProjectAs				();
	void PrjectSetting				();
	void CloseProject				();
	void Compute					();
	void onChangeMode				();

protected:
	void resizeEvent ( QResizeEvent * event );

};

#endif // CARGOLOADING_H
