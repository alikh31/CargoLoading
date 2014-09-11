#include "addcarrier.h"

Addcarrier::Addcarrier(QWidget *parent,CGUIMainWindow* pMainWindow)
: QDialog(parent)
{
	m_pMainWindow = pMainWindow;

	setupUi(this);

	connect(pushButton, SIGNAL(clicked()), this, SLOT(onClickedAccept()));

	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
    setFixedSize(406, 186);

}

Addcarrier::~Addcarrier()
{

}

void Addcarrier::onClickedAccept()
{
	lineEdit->displayText();
	isAccepted = true;
	__Carrier* temp = new __Carrier();
	temp->id=lineEdit->displayText().toInt();
	if (temp->id==NULL)
	
	{
		int ret = QMessageBox::warning(this, QString("Warning!"),QString("the code is not valid"));//"error","code mojod nist"
		return;
	}
    temp->name=lineEdit_2->displayText();
	if (temp->name==NULL)
	{
		int ret = QMessageBox::warning(this, QString("Warning!"),QString("the name is not valid"));//"error","name does not found"
		return;
	}
	temp->Length=lineEdit_3->displayText().toInt();
	temp->Width=lineEdit_4->displayText().toInt();
	temp->Height=lineEdit_5->displayText().toInt();
	if (temp->Length==NULL ||temp->Width==NULL||temp->Height==NULL)
	{
	    int ret=QMessageBox::warning(this,QString("Warning!"),QString("Height width or depth is invalid"));//"tol ya arz ya ertefae vared nashodeh ast"
	    return;
	}
    temp->MaxLoad=lineEdit_6->displayText().toInt();
	if (temp->MaxLoad==NULL)
	{ 
       int ret =QMessageBox::warning(this,QString("Warning!"),QString("Max weight is invalid"));//"bishineh bar vared nashodeh ast"
	   return;
	}
	temp->CostPerKm=lineEdit_7->displayText().toInt();
	if (temp->CostPerKm==NULL)
	{
        int ret=QMessageBox::warning(this,QString("Warning!"),QString("the cost is invalid"));//"gheimat vared nashodeh ast mojod nist"
	    return;
	}
	temp->VahicleType=lineEdit_8->displayText().toInt();
	if(temp->VahicleType==NULL)
	{
        int ret=QMessageBox::warning(this,QString("Warning!"),QString("Type of carrier is invalid"));//"noe hamel mojod nist"
	    return;
	}
	m_pMainWindow->m_pProject->m_CarrierList.append(temp);

	close();

}