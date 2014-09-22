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

    __Order temp;

    QDate GDate(2014 , 1 , 1);

    GDate = GDate.addDays(temp.minTime);
    lineEdit_2->setText(QString("%1/%2/%3").arg(GDate.day() ).arg(GDate.month()).arg(GDate.year()));

    GDate = QDate(2014 , 1 , 1);
    GDate = GDate.addDays(temp.maxTime);
    lineEdit_3->setText(QString("%1/%2/%3").arg(GDate.day() ).arg(GDate.month()).arg(GDate.year()));
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



    QString date = lineEdit_2->displayText();
    QStringList dateList = date.split("/");
    int day = dateList[0].toInt();
    int month = dateList[1].toInt();
    int year = dateList[2].toInt();

    QDate GDate(year , month , day);

    int minTime=QDate(2014,1,1).daysTo(GDate);

    if( day>31 || day < 1 || month < 1 || month > 12 || year < 2014)
    {
        int ret = QMessageBox::warning(this, QString("Warning!"),QString("min time is invalid"));
        return;
    }

    temp->minTime = minTime;

    //////////////////////////////////////////////////////////////////////////

    date = lineEdit_3->displayText();
    dateList = date.split("/");
    day = dateList[0].toInt();
    month = dateList[1].toInt();
    year = dateList[2].toInt();

    GDate = QDate(year , month , day);

    int maxTime=QDate(2014,1,1).daysTo(GDate);

    if( day>31 || day < 1 || month < 1 || month > 12 || year < 2014)
    {
        int ret = QMessageBox::warning(this, QString("Warning!"),QString("max time is invalid"));
        return;
    }

    temp->maxTime = maxTime;

    //////////////////////////////////////////////////////////////////////////

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