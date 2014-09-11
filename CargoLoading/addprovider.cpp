#include "addprovider.h"

AddProvider::AddProvider(QWidget *parent,CGUIMainWindow* pMainWindow)
	: QDialog(parent)
{
	m_pMainWindow = pMainWindow;

	dss a(10,12);
	int max = a.mmax();

	setupUi(this);

	connect(pushButton, SIGNAL(clicked()), this, SLOT(onClickedAccept()));

	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(close()));

	setFixedSize(187, 217);
	isAccepted = false;
}

AddProvider::~AddProvider()
{

}


void AddProvider::onClickedAccept()
{

	lineEdit->displayText();

	isAccepted = true;
	__Provider* temp = new __Provider();

	 temp->id= lineEdit->displayText().toInt();
	 if(temp->id==NULL)

	 {
		 int ret = QMessageBox::warning(this, QString("Warning!"),QString("code is invalid"));//"error","code mojod nist"
		 return;
	 }

	if(lineEdit_2->displayText()==NULL)
	{
		int ret = QMessageBox::warning(this, QString("Warning!"),QString("name does not found"));//"error","name does not found"
		return;
	}
		
	temp->name=lineEdit_2->displayText();
	temp->Address=lineEdit_3->displayText();
	if (temp->Address==NULL)

	{int ret=QMessageBox::warning(this,QString("Warning!"),QString("Address does not found"));//"error","address mojod nist"
	return;
	}
	temp->phi=lineEdit_4->displayText().toInt();
	temp->landa=lineEdit_5->displayText().toInt();
	if (temp->landa==NULL || temp->phi==NULL )
	{
		int ret=QMessageBox::warning(this,QString("Warning!"),QString("Location does not found"));//"error","mogheiat mojod nist"
		return;
	}

	m_pMainWindow->m_pProject->m_ProviderList.append(temp);

	close();
}