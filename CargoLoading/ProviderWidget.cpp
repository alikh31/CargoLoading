#include "ProviderWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
//#include <QSpacerItem >
#include <addprovider.h>

CProviderWidget::CProviderWidget(QString title , QWidget *parent,CGUIMainWindow* pMainWindow)
: QDockWidget(title , parent)
{  
	m_pMainWindow = pMainWindow;
	m_pTable=new CProviderTable(parent,pMainWindow);
	QVBoxLayout* vBoxLayout=new QVBoxLayout;
	QHBoxLayout* hBoxLayout=new QHBoxLayout;
	vBoxLayout->setContentsMargins(0,0,0,0);
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addWidget(m_pTable);
	/*m_pNewRow=new QPushButton;
	m_pNewRow->setMaximumHeight(100);
	m_pNewRow->setMaximumWidth(200);
	hBoxLayout->addWidget(m_pNewRow)*/;
	//connect(m_pNewRow,SIGNAL(clicked ( bool )),this,SLOT(OnAddNewRow( bool )));
	m_pNewRow=new QPushButton;
	m_pClearAll=new QPushButton;
	SetupButton(QString("Add New Row"),m_pNewRow,hBoxLayout);//"Add New Row"
	SetupButton(QString("Clear All"),m_pClearAll,hBoxLayout);//"ClearAll"
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

CProviderWidget::~CProviderWidget()
{

}

void CProviderWidget::SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout)
{
	pButton->setText(Text);
	pButton->setMaximumHeight(100);
	pButton->setMaximumWidth(200);
	pLayout->addWidget(pButton);

}

void CProviderWidget::OnAddNewRow(bool bOk)

{
	AddProvider* pAddProvider = new AddProvider(0, m_pMainWindow);

	pAddProvider->exec();

	m_pTable->AddNewRowEvent();


}

void CProviderWidget::OnClearAllRows(bool bOk)
{

}
