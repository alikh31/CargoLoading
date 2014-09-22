#include "addcargo.h"

AddCargo::AddCargo(QWidget *parent,CGUIMainWindow* pMainWindow)
	: QDialog(parent)
{
	m_pMainWindow = pMainWindow;

	setupUi(this);
	
	connect(pushButton, SIGNAL(clicked()), this, SLOT(onClickedAccept()));

	connect(pushButton_2, SIGNAL(clicked()), this, SLOT(onClickedReject()));
	
	//setFixedSize(412, 230);
	
	isAccepted = false;





	
    __BoxRow temp;

	lineEdit->setText(tr("%1").arg(temp.id));
	lineEdit->setReadOnly(true);
	lineEdit_11->setText(temp.Color);
	lineEdit_2->setText(temp.Desc);
	lineEdit_8->setText(temp.FloorIndex);
	lineEdit_10->setText(tr("%1").arg(temp.MaxLoad));
	lineEdit_14->setText(tr("%1").arg(temp.PaleteSizeH));
	lineEdit_13->setText(tr("%1").arg(temp.PaleteSizeD));
	lineEdit_12->setText(tr("%1").arg(temp.PaleteSizeW));
	lineEdit_9->setText(tr("%1").arg(temp.Priority));
	lineEdit_7->setText(tr("%1").arg(temp.PaleteWeight));



}

AddCargo::~AddCargo()
{

}

void AddCargo::onClickedAccept()
{   lineEdit->displayText();
	isAccepted = true;
	__BoxRow* temp = new __BoxRow();



	
	temp->Color=lineEdit_11->displayText();
	
	temp->Desc= lineEdit_2->displayText();
	if(temp->Desc==NULL)
    {
        int ret = QMessageBox::warning(this, QString("Warning!"),QString( "Description is Empty"));
        return;
    }
    temp->FloorIndex=lineEdit_8->displayText();	
    temp->MaxLoad=lineEdit_10->displayText().toInt();
    if(temp->MaxLoad==NULL)
    {
        int ret = QMessageBox::warning(this, QString("Warning!"),QString( "Max weight has been enter wrong"));
        return;
    }

    temp->PaleteSizeH=lineEdit_14->displayText().toInt();
    temp->PaleteSizeD=lineEdit_13->displayText().toInt();
    temp->PaleteSizeW=lineEdit_12->displayText().toInt();
    temp->Priority=lineEdit_9->displayText().toInt();
    temp->PaleteWeight=lineEdit_7->displayText().toInt();
    if(temp->PaleteSizeH == 0 || temp->PaleteSizeD == 0 || temp->PaleteSizeW == 0 || temp->PaleteWeight == 0)
    {
        int ret = QMessageBox::warning(this, QString("Warning!"),QString( "Height, Width or weight has been enter wrong"));
        return;
    }


	m_pMainWindow->m_pProject->m_CargoList.append(temp);

close();
}

void AddCargo::onClickedReject()
{
    close();
}
