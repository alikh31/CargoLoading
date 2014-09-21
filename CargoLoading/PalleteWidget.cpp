#include "PalleteWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
//#include <QSpacerItem >

CPalleteWidget::CPalleteWidget(QString title , QWidget *parent,CGUIMainWindow* pMainWindow)
: QDockWidget(title , parent)
{
	m_pTable=new CPalleteTable(parent,pMainWindow);
	QVBoxLayout* vBoxLayout=new QVBoxLayout;
	QHBoxLayout* hBoxLayout=new QHBoxLayout;
	vBoxLayout->setContentsMargins(0,0,0,0);
	vBoxLayout->addLayout(hBoxLayout);
	vBoxLayout->addWidget(m_pTable);
	m_pNewRow=new QPushButton;
	m_pClearAll=new QPushButton;
	SetupButton(QString("Add New Row"),m_pNewRow,hBoxLayout);//"Add New Row"
	connect(m_pNewRow, SIGNAL(clicked ( bool )),this, SLOT(OnAddNewRow(bool)));
	SetupButton(QString("Clear All"),m_pClearAll,hBoxLayout);//"ClearAll"
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

CPalleteWidget::~CPalleteWidget()
{

}

void CPalleteWidget::SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout)
{
	pButton->setText(Text);
	pButton->setMaximumHeight(100);
	pButton->setMaximumWidth(200);
	pLayout->addWidget(pButton);

}

void CPalleteWidget::OnAddNewRow(bool bOk)
{
	m_pTable->AddNewRowEvent();

}

void CPalleteWidget::OnClearAllRows(bool bOk)
{

}
