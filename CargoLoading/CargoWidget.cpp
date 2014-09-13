#pragma region "Includes"
#include "CargoWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
//#include <QSpacerItem >
#include "addcargo.h"
#pragma endregion

#pragma region "Constructor"
CCargoWidget::CCargoWidget(QString title , QWidget *parent,CGUIMainWindow* pMainWindow)
	: QDockWidget(title , parent)
{
	m_pMainWindow = pMainWindow;
	m_pTable=new CCargoTable(parent,pMainWindow);
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
	SetupButton(QString("ClearAll"),m_pClearAll,hBoxLayout);//"ClearAll"
	connect(m_pNewRow, SIGNAL(clicked ( bool )),this, SLOT(OnAddNewRow(bool)));
	connect(m_pClearAll, SIGNAL(clicked ( bool )),this, SLOT(OnClearAllRows(bool)));
	QSpacerItem *horizontalSpacer;
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	hBoxLayout->addSpacerItem(horizontalSpacer );

	
	

	QWidget* Widget = new QWidget;
	Widget->setLayout(vBoxLayout);

	setWidget(Widget);


	//setMinimumWidth(m_pMainWindow->width()/2);

	m_pMainWindow->m_fFirstTime = true;
}
#pragma endregion

#pragma region "DeConstructor"
CCargoWidget::~CCargoWidget()
{

}
#pragma endregion

#pragma region "void CCargoWidget::SetupButton(QString Text,QPushButton* pButton,QString iconPath)"
void CCargoWidget::SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout)
{
	pButton->setText(Text);
	pButton->setMaximumHeight(100);
	pButton->setMaximumWidth(200);
	pLayout->addWidget(pButton);

}
#pragma endregion

#pragma region "void CCargoWidget::OnAddNewRow()"
void CCargoWidget::OnAddNewRow(bool bOk)
{
	AddCargo pAddCargo(0, m_pMainWindow);

	pAddCargo.exec();

	m_pTable->AddNewRowEvent();

}
#pragma endregion

#pragma region "void CCargoWidget::OnClearAllRows()"
void CCargoWidget::OnClearAllRows(bool bOk)
{

}
#pragma endregion
