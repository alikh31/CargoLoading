#include "CarrierWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
//#include <QSpacerItem >
#include "addcarrier.h"

CCarrierWidget::CCarrierWidget(QString title , QWidget *parent,CGUIMainWindow* pMainWindow)
	: QDockWidget(title , parent)
{   m_pMainWindow = pMainWindow;
	m_pTable=new CCarrierTable(parent,pMainWindow);
	QVBoxLayout* vBoxLayout=new QVBoxLayout;
	QHBoxLayout* hBoxLayout=new QHBoxLayout;
	vBoxLayout->setContentsMargins(0,0,0,0);
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addWidget(m_pTable);
	m_pNewRow=new QPushButton;
	m_pClearAll=new QPushButton;
	SetupButton(QString("Add new row"),m_pNewRow,hBoxLayout);//"add new row"
	SetupButton(QString("Clear All"),m_pClearAll,hBoxLayout);//"Clear All"	
	connect(m_pNewRow, SIGNAL(clicked ( bool )),this, SLOT(OnAddNewRow(bool)));
	connect(m_pClearAll, SIGNAL(clicked ( bool )),this, SLOT(OnClearAllRows(bool)));
	QSpacerItem *horizontalSpacer;
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	hBoxLayout->addSpacerItem(horizontalSpacer );


	//setLayout(vBoxLayout);


	QWidget* Widget = new QWidget;
	Widget->setLayout(vBoxLayout);

	setWidget(Widget);

	//setMinimumWidth(pMainWindow->width()/2);

	pMainWindow->m_fFirstTime = true;
}

CCarrierWidget::~CCarrierWidget()
{

}

void CCarrierWidget::SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout)
{
	pButton->setText(Text);
	pButton->setMaximumHeight(100);
	pButton->setMaximumWidth(200);
	pLayout->addWidget(pButton);

}

void CCarrierWidget::OnAddNewRow(bool bOk)
{
	Addcarrier* pAddcarrier = new Addcarrier(0, m_pMainWindow);
    pAddcarrier->exec();
	m_pTable->AddNewRowEvent();
    
}

void CCarrierWidget::OnClearAllRows(bool bOk)
{

}
