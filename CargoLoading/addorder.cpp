#include "addorder.h"

AddOrder::AddOrder(QWidget *parent,CGUIMainWindow* pMainWindow)
	: QDialog(parent)
{
	m_pMainWindow = pMainWindow;

	setupUi(this);

	connect(pushButton, SIGNAL(clicked()), this, SLOT(onClickedAccept()));

	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(close()));

	setFixedSize(208, 224);
	isAccepted = false;
}

AddOrder::~AddOrder()
{

}


void AddOrder::onClickedAccept()
{
	lineEdit->displayText();
	
	isAccepted = true;

	__Order* temp = new __Order();

	if(m_pMainWindow->m_pProject->GetBoxByName(lineEdit->displayText())==NULL)
	{
		    int ret = QMessageBox::warning(this, QString("Warning!"),QString("Name is invalid"));//"error","name does not found"
		return;
	}

	temp->BoxID = m_pMainWindow->m_pProject->GetBoxByName(lineEdit->displayText())->id;

	temp->maxTime = lineEdit_2->displayText().toInt();
	temp->minTime = lineEdit_3->displayText().toInt();
	temp->count = lineEdit_4->displayText().toInt();


	if(temp->maxTime == 0 || temp->minTime == 0 || temp->maxTime < temp->minTime )
	{
		int ret = QMessageBox::warning(this, QString("Warning!"),QString("Time is invalid"));//"error","zaman eshtebah vared shodeh ast"
		return;
	}

	if(temp->count==0)
	{
		int ret = QMessageBox::warning(this, QString("Warning!"),QString("Num is invalid"));//"error","tedad eshtebah vared shodeh ast"
		return;
	}


	if(m_pMainWindow->m_pProject->GetProviderByName(lineEdit_5->displayText())==NULL)
	{
		int ret = QMessageBox::warning(this, QString("Warning!"),QString("provider does not found"));//"error","provider does not found"
		return;
	}


	temp->pProvider = m_pMainWindow->m_pProject->GetProviderByName(lineEdit_5->displayText());

	temp->ProviderID = m_pMainWindow->m_pProject->GetProviderByName(lineEdit_5->displayText())->id;

	m_pMainWindow->m_pProject->m_OrderList.append(temp);

	close();
}