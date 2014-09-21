#include "GUIMainWindow.h"
#include "CargoWidget.h"
#include "ProviderWidget.h"
#include "CarrierWidget.h"
#include "PalleteWidget.h"
#include "OrderWidget.h"
#include "PlanTree.h"
#include <qmenubar>
#include <QFileInfo>
#include <QFileDialog>
#include <QSplitter>

CGUIMainWindow::CGUIMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_fFirstTime = false;
	m_pProject=new CProject;
	setWindowIcon(QIcon("Resources/Ever-icon2.png"));
	setWindowTitle("Cargo Loading Planner .");
	setMinimumWidth(800);
	setMinimumHeight(600);
	showMaximized();
	setupMenuBar();
	setupCentralWidget();
	setupDockWidget();
    //setLayoutDirection(Qt::RightToLeft);
}

CGUIMainWindow::~CGUIMainWindow()
{
	delete m_pProject;
}

void CGUIMainWindow::setupCentralWidget()
{
	center = new QTabWidget (this);
	
	m_pCargoTab= new CCargoWidget(QString("Cargo") ,this,this);
	m_pCargoTab->setObjectName(QString("Cargo"));//Cargo
	addDockWidget(Qt::RightDockWidgetArea , m_pCargoTab);
	m_pViewMenu->addAction(m_pCargoTab->toggleViewAction());

	m_pProviderTab= new CProviderWidget( QString("provider") , this,this);
	m_pProviderTab->setObjectName(QString("provider"));//2-provider
	tabifyDockWidget(m_pCargoTab , m_pProviderTab);
	m_pViewMenu->addAction(m_pProviderTab->toggleViewAction());

	m_pPalleteRelationTab= new CPalleteWidget( QString("pallet relation") , this,this);
	m_pPalleteRelationTab->setObjectName(QString("pallet relation"));//pallet relation
	tabifyDockWidget(m_pProviderTab , m_pPalleteRelationTab);
	m_pViewMenu->addAction(m_pPalleteRelationTab->toggleViewAction());

	m_pCarriersTab= new CCarrierWidget( QString("Carriers") ,this,this);
	m_pCarriersTab->setObjectName(QString("CarriersTab"));//4-pCarriersTab
	tabifyDockWidget(m_pPalleteRelationTab , m_pCarriersTab);
	m_pViewMenu->addAction(m_pCarriersTab->toggleViewAction());

	m_pCurrentOrderListTab= new COrderWidget(QString("Current Order"), this,this);
	m_pCurrentOrderListTab->setObjectName(QString("Current Order"));//5-Current Order
	
	addDockWidget(Qt::LeftDockWidgetArea , m_pCurrentOrderListTab);
	m_pViewMenu->addAction(m_pCurrentOrderListTab->toggleViewAction());

	m_pPlanWidget = new QDockWidget(QString("Final Planning") , this);

	/*m_pNetworkWidget=new CNetworkWidget(m_pProject,this,this);
	m_pNetworkWidget->resize(this->size().width() /3,this->size().height() /2);*/
    m_pGLWidget = new GLWidget(this);
    m_pGLWidget->resize(this->size().width() /3,this->size().height() /2);
    //m_pMainWidget->show();

	m_pPlanTree= new CPlanTree(this,this,m_pProject);
	QSplitter *CurrentOrderSplitter = new QSplitter;
	CurrentOrderSplitter->setOrientation(Qt::Vertical);
	CurrentOrderSplitter->addWidget(m_pPlanTree);
	CurrentOrderSplitter->addWidget(m_pGLWidget);
	m_pPlanTree->setObjectName(QString("All Orders"));//"6-All Orders"
	m_pPlanWidget->setWidget(CurrentOrderSplitter);
	m_pPlanWidget->setMinimumWidth(this->width()/2);
	tabifyDockWidget(m_pCurrentOrderListTab , m_pPlanWidget);
	m_pViewMenu->addAction(m_pPlanWidget->toggleViewAction());


	m_pCurrentPlanTab= new QTableView();
	m_pCurrentPlanTab->setObjectName(QString("Current Plans"));//7-Current Plans

	/*m_pCargoTab->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_pProviderTab->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_pPalleteRelationTab->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_pCarriersTab->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_pCurrentOrderListTab->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_pPlanWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);*/

    return;
}

void CGUIMainWindow::setupDockWidget()
{
	/*QMainWindow::setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	QMainWindow::setCorner(Qt::BottomLeftCorner , Qt::LeftDockWidgetArea);
	QMainWindow::setCorner(Qt::TopRightCorner , Qt::RightDockWidgetArea );
	QMainWindow::setCorner(Qt::BottomRightCorner , Qt::RightDockWidgetArea );

	m_pShortkeyDV=new CShortKeysDV(this);
	m_pShortkeyDV->setWindowTitle("Short Keys");*/
	//addDockWidget(Qt::RightDockWidgetArea ,m_pShortkeyDV);

}

void CGUIMainWindow::setupMenuBar()
{
	QMenu *menu = menuBar()->addMenu(QString("&File"));//"&File"

	QAction *action ;
	m_pNewProjAction= menu->addAction(QIcon("Resources/new.png"),QString("&New"));//"New "
	m_pNewProjAction->setShortcut(tr("Ctrl+N"));
	connect(m_pNewProjAction, SIGNAL(triggered()), this, SLOT(NewProject()));

	m_pOpenProjAction = menu->addAction(QIcon("Resources/open.png"),QString("&Open Project"));//"open project"
	m_pOpenProjAction ->setShortcut(tr("Ctrl+O"));
	connect(m_pOpenProjAction, SIGNAL(triggered()), this, SLOT(OpenProject()));
	menu->addSeparator();

	m_pSaveProjAction = menu->addAction(QIcon("Resources/save.png"),QString("Save"));//"Save"	m_pSaveProjAction  ->setShortcut(tr("Ctrl+S"));
	connect(m_pSaveProjAction, SIGNAL(triggered()), this, SLOT(SaveProject()));

	action = menu->addAction(QString("Save as..."));//"Save As"
	connect(action, SIGNAL(triggered()), this, SLOT(SaveProjectAs()));
	menu->addSeparator();

	menu->addAction(QString("Close"), this, SLOT(CloseProject()));//"Close"

	menu->addSeparator();

	menu->addAction(QString("Setting"), this, SLOT(PrjectSetting()));//"Setting"

	menu->addSeparator();


	action = menu->addAction(QString("&Exit"), this, SLOT(close()));//"&Exit"
	action->setShortcut(tr("Ctrl+E"));

	menu  = menuBar()->addMenu(QString("Tools"));//"Tools"
	
	m_pComputeAction = menu->addAction(QString("Compute"));//"Compute"
	m_pComputeAction ->setShortcut(tr("Ctrl+C"));
	connect(m_pComputeAction, SIGNAL(triggered()), this, SLOT(Compute()));

	menu->addSeparator();

	QActionGroup* alignmentGroup = new QActionGroup(this);

	m_pSequentialModeAction = alignmentGroup->addAction("Sequential Mode");
	m_pSequentialModeAction->setCheckable(true);
	connect(m_pSequentialModeAction, SIGNAL(triggered()), this, SLOT(onChangeMode()));

	m_pOptimizedModeAction = alignmentGroup->addAction("Optimized Mode");
	m_pOptimizedModeAction->setCheckable(true);
	m_pOptimizedModeAction->setChecked(true);
	connect(m_pOptimizedModeAction, SIGNAL(triggered()), this, SLOT(onChangeMode()));

	menu->addAction(m_pOptimizedModeAction );
	menu->addAction(m_pSequentialModeAction);

	m_pViewMenu = menuBar()->addMenu(QString("View"));

	menu  = menuBar()->addMenu(QString("Help"));//"Help"

	action= menu->addAction(QString("Help"));//"Help"
	action->setShortcut(tr("Ctrl+H"));
	connect(action, SIGNAL(triggered()), this, SLOT(Help()));

	menu->addSeparator();

	action= menu->addAction(QString("About..."));//"About"
	action->setShortcut(tr("Ctrl+A"));
	connect(action, SIGNAL(triggered()), this, SLOT(About()));
}

void CGUIMainWindow::NewProject()
{
	m_pProject->newProject();
	setWindowTitle("Cargo Loading Planner .");
	UpdateAll();
}

void CGUIMainWindow::OpenProject()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		"Select project File",
		"/home",
		"Project File (*.prj );;All Files (*.*)");
	QFileInfo fi(fileName);
	if(fi.exists())
	{
		if(m_pProject->Open(fileName))
		{
			setWindowTitle(m_pProject->m_filePath + " - Cargo Loading Planner .");
			//m_pCargoTab->m_pTable->ReLoadModel();
			//m_pProviderTab->m_pTable->ReLoadModel();
			//m_pPalleteRelationTab->m_pTable->ReLoadModel();
			//m_pCarriersTab->m_pTable->ReLoadModel();
			m_pCurrentOrderListTab->m_pTable->ReLoadModel();

		}
	}
    m_pGLWidget->m_pProject = m_pProject;
	return;
}

void CGUIMainWindow::SaveProject()
{
	QFileInfo info(m_pProject->m_filePath);
	if(info.exists())
	{
		QString file=m_pProject->m_filePath;
		QFileInfo info2(file);
		QString suffix=info2.suffix();
		if(suffix.size()==0)
			file+=".prj";
		m_pProject->SaveAs(file);
		setWindowTitle(m_pProject->m_filePath+" - Cargo Loading Planner .");
	}
	else 
		SaveProjectAs();
}

void CGUIMainWindow::SaveProjectAs()
{
	QFileDialog dlg(this);
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	dlg.setNameFilter( "Project File (*.prj );;All File (*.* )");
	if(dlg.exec())
	{
		QString file=dlg.selectedFiles().at(0);
		QFileInfo info2(file);
		QString suffix=info2.suffix();
		if(suffix.size()==0)
			file+=".prj";
		m_pProject->SaveAs(file);
		setWindowTitle(m_pProject->m_filePath+" - Cargo Loading Planner .");
	}

}

void CGUIMainWindow::CloseProject()
{
	m_pProject->closeProject();
	setWindowTitle("Cargo Loading Planner .");
}

void CGUIMainWindow::PrjectSetting()
{
	/*CProjectSettingWidget widget(this,m_pProject);
	((QWidget*)&widget)->setWindowTitle("Setting")s;
	widget.exec();*/
}

void CGUIMainWindow::UpdateAll()
{
	m_pCargoTab->m_pTable->m_pModel->RefreshList();
	m_pProviderTab->m_pTable->m_pModel->RefreshList();
	m_pPalleteRelationTab->m_pTable->m_pModel->RefreshList();
	m_pCarriersTab->m_pTable->m_pModel->RefreshList();
	m_pCurrentOrderListTab->m_pTable->m_pModel->RefreshList();

}

void CGUIMainWindow::Compute()
{
	m_pProject->Compute();
	m_pPlanTree->CreateBody();
}

void CGUIMainWindow::onChangeMode()
{
	m_pProject->m_fMode = m_pSequentialModeAction->isChecked();
}

void CGUIMainWindow::resizeEvent ( QResizeEvent * event )
{
	if(m_fFirstTime)	
	{/*
		m_pCargoTab->setMinimumWidth(event->size().width()/2);
		m_pCargoTab->resize(event->size().width() /2, m_pCargoTab->height());

		m_pProviderTab->setMinimumWidth(event->size().width()/2);
		m_pProviderTab->resize(event->size().width() /2, m_pProviderTab->height());

		m_pPalleteRelationTab->setMinimumWidth(event->size().width()/2);
		m_pPalleteRelationTab->resize(event->size().width() /2, m_pPalleteRelationTab->height());

		m_pCarriersTab->setMinimumWidth(event->size().width()/2);
		m_pCarriersTab->resize(event->size().width() /2, m_pCarriersTab->height());

		m_pCurrentOrderListTab->setMinimumWidth(event->size().width()/2);
		m_pCurrentOrderListTab->resize(event->size().width() /2, m_pCurrentOrderListTab->height());

		m_pPlanWidget->setMinimumWidth(event->size().width()/2);
		m_pPlanWidget->resize(event->size().width() /2, m_pPlanWidget->height());*/

	}
}
