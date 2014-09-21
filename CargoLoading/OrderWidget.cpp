#include "OrderWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
//#include <QSpacerItem >
#include "PlanTree.h"
#include "addorder.h"
#include <QMessageBox>
//#include "NetworkWidget.h"

COrderWidget::COrderWidget(QString title , QWidget *parent,CGUIMainWindow* pMainWindow)
: QDockWidget(title , parent)
{
	m_pMainWindow=pMainWindow;
	m_pTable=new COrderTable(parent,pMainWindow);
	QVBoxLayout* vBoxLayout=new QVBoxLayout;
	QHBoxLayout* hBoxLayout=new QHBoxLayout;
	vBoxLayout->setContentsMargins(0,0,0,0);
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addWidget(m_pTable);
	m_pNewRow=new QPushButton;
	m_pClearAll=new QPushButton;
	m_pCompute=new QPushButton;
	SetupButton(QString("Add New Row"),m_pNewRow,hBoxLayout);//"Add New Row"
	SetupButton(QString("ClearAll"),m_pClearAll,hBoxLayout);//"ClearAll"
	SetupButton(QString("Compute"),m_pCompute,hBoxLayout);//"Compute"
	connect(m_pNewRow, SIGNAL(clicked ( bool )),this, SLOT(OnAddNewRow(bool)));
	connect(m_pClearAll, SIGNAL(clicked ( bool )),this, SLOT(OnClearAllRows(bool)));
	connect(m_pCompute, SIGNAL(clicked ( bool )),this, SLOT(OnCompute(bool)));
	QSpacerItem *horizontalSpacer;
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	hBoxLayout->addSpacerItem(horizontalSpacer );
	//setLayout(vBoxLayout);

	QWidget* Widget = new QWidget;
	Widget->setLayout(vBoxLayout);

	setWidget(Widget);

	//setMinimumWidth(m_pMainWindow->width()/2);

	m_pMainWindow->m_fFirstTime = true;
}

COrderWidget::~COrderWidget()
{

}

void COrderWidget::SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout)
{
	pButton->setText(Text);
	pButton->setMaximumHeight(100);
	pButton->setMaximumWidth(200);
	pLayout->addWidget(pButton);

}

void COrderWidget::OnAddNewRow(bool bOk)
{
	AddOrder* pAddOrder = new AddOrder(NULL,m_pMainWindow);

	pAddOrder->exec();

	m_pTable->AddNewRowEvent();

}

void COrderWidget::OnClearAllRows(bool bOk)
{

}

void COrderWidget::OnCompute(bool bOk)
{
	m_pMainWindow->m_pProject->Compute();
	m_pMainWindow->m_pPlanTree->CreateBody();
}
