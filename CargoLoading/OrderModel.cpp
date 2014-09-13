#pragma region "Includes"
#include "OrderModel.h"
#include "GUIMainWindow.h"
//#include "JalaliCalendar.h"


#pragma endregion

#pragma region "Constructor"
COrderModel::COrderModel(CGUIMainWindow* pMainWindow)
: QAbstractTableModel()
{
	m_pMainWindow=pMainWindow;
}
#pragma endregion

#pragma region "DeConstructor"
COrderModel::~COrderModel()
{

}
#pragma endregion

#pragma region "int COrderModel::rowCount(const QModelIndex & ) const "
int COrderModel::rowCount(const QModelIndex & ) const 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;
	int count=m_pMainWindow->m_pProject->m_OrderList.count();
	return count;
}
#pragma endregion

#pragma region "int COrderModel::columnCount(const QModelIndex & ) const "
int COrderModel::columnCount(const QModelIndex & ) const 
{
	return 5;
}
#pragma endregion

#pragma region "void COrderModel::RefreshList()"
void COrderModel::RefreshList()
{
    endResetModel();
}
#pragma endregion

#pragma region "QVariant COrderModel::data(const QModelIndex &index, int role) const "
QVariant COrderModel::data(const QModelIndex &index, int role) const 
{

	if(!index.isValid()) return QVariant();
	if (role == Qt::BackgroundColorRole)
	{
		//return (QVariant) ( QColor( Qt::yellow ) );
	}
	if(role==Qt::SizeHintRole)
	{
		int test=10;
	}

	if (role == Qt::FontRole)
	{
		if(index.column() == 0) {
			QFont font;
			font.setBold(false);
			return font;
		}
	}

	if (role == Qt::TextAlignmentRole)
	{
		if(index.column() == 0) 
		{
			return (QVariant) ( Qt::AlignLeft  | Qt::AlignVCenter );
		} else {
			return (QVariant) 
				( 
				Qt::AlignHCenter );
		}
	}

	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;

	if (role == Qt::DisplayRole || role == Qt::EditRole) 
	{
		int col=index.column();
		int row=index.row();

		if(row<0 || row>=m_pMainWindow->m_pProject->m_OrderList.count())
			return QVariant();

		__Order* rowData=m_pMainWindow->m_pProject->m_OrderList.at(row);
		__BoxRow* box=m_pMainWindow->m_pProject->GetBox(rowData->BoxID);
		__Provider* provider=m_pMainWindow->m_pProject->GetProvider(rowData->ProviderID);
		QString tmp;
		QDate GDate(1360 , 1 , 1);			
        //CJalaliCalendar calender;
		int day , month , year;

		switch(col)
		{
		/*case 0:
			return rowData->id;*/
		case 0:
			if(box)
				return box->Desc;
			else
				return QVariant();
		case 1:
			return rowData->count;
		case 2:
			GDate = GDate.addDays(rowData->minTime);
			//calender.GregorianToJalali(GDate.year() ,GDate.month() , GDate.day() , &year ,&month , &day);
			return QString("%1/%2/%3").arg(GDate.day() ).arg(GDate.month()).arg(GDate.year());
		case 3:
			GDate = GDate.addDays(rowData->maxTime);
			//calender.GregorianToJalali(GDate.year() ,GDate.month() , GDate.day() , &year ,&month , &day);
			return QString("%1/%2/%3").arg(GDate.day() ).arg(GDate.month()).arg(GDate.year());
		case 4:
			if(provider)
				return provider->name;
			else
				return QVariant();
		}
	}
	return QVariant();
}
#pragma endregion

#pragma region "bool COrderModel::setData(const QModelIndex &index, const QVariant &value, int role) "
bool COrderModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;

	if (index.isValid() && role == Qt::EditRole)
	{
		// we have index.row() index.column() and value
		// all we need
		int col=index.column();
		int row=index.row();

		if(row<0 || row>=m_pMainWindow->m_pProject->m_OrderList.count())
			return false;

		__Order* rowData=m_pMainWindow->m_pProject->m_OrderList.at(row);

		/*if(col==0)
		{
			return false;
		}
		else */
		if(col==0)
		{
			QString name=value.toString();
			__BoxRow* box=m_pMainWindow->m_pProject->GetBoxByName(name);
			if(box)
			{
				rowData->BoxID=box->id;
				return true;
			}
			else
				return false;
		}
		else if(col==1)
		{
			bool bOk;
			int count=value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->count=count;
		}
		else if(col==2)
		{
			bool bOk;
			QString date=value.toString();
			QStringList dateList = date.split("/");
			int day = dateList[0].toInt(&bOk);
			int month = dateList[1].toInt(&bOk);
			int year = dateList[2].toInt(&bOk);

			QDate GDate(year , month , day);

			int minTime=QDate(1360,1,1).daysTo(GDate);

			if(!bOk || day>31 || day < 1 || month < 1 || month > 12 || year < 1360)
				return false;

			rowData->minTime=minTime;
		}
		else if(col==3)
		{
			bool bOk;
			QString date=value.toString();
			QStringList dateList = date.split("/");
			int day = dateList[0].toInt(&bOk);
			int month = dateList[1].toInt(&bOk);
			int year = dateList[2].toInt(&bOk);

			QDate GDate(year , month , day);

			int maxTime=QDate(1360,1,1).daysTo(GDate);

			if(!bOk || day>31 || day < 1 || month < 1 || month > 12 || year < 1360)
				return false;

			rowData->maxTime=maxTime;
		}
		else if(col==4)
		{
			QString name=value.toString();
			__Provider* provider=m_pMainWindow->m_pProject->GetProviderByName(name);
			if(provider)
			{
				rowData->ProviderID=provider->id;
				rowData->Provider = provider;
				return true;
			}
			else
				return false;
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}
#pragma endregion

#pragma region "Qt::ItemFlags COrderModel::flags(const QModelIndex &index) const "
Qt::ItemFlags COrderModel::flags(const QModelIndex &index) const 
{
	if (!index.isValid()) return Qt::ItemIsEnabled;
	int col=index.column();
	int row=index.row();
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}
#pragma endregion

#pragma region "QVariant COrderModel::headerData(int section, Qt::Orientation orientation, int role) const  "
QVariant COrderModel::headerData(int section, Qt::Orientation orientation, int role) const  
{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation == Qt::Horizontal) 
	{
		switch(section)
		{
		/*case 0:
			return QString("Code");//"Code"*/
		case 0:
			return QString("pallete name");//"pallete name"
		case 1:
			return QString("count");//"count"
		case 2:
			return QString("min time");//"mintime"
		case 3:
			return QString("max time");//"maxtime"
		case 4:
			return QString("provider name");//"providername"
		}
	}
	else 
	{
		QString headerName=QString("%1").arg(section);
		return headerName;

	}
	return "";
}
#pragma endregion

