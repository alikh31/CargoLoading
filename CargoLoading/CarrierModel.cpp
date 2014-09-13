#pragma region "Includes"
#include "CarrierModel.h"
#include "GUIMainWindow.h"


#pragma endregion

#pragma region "Constructor"
CCarrierModel::CCarrierModel(CGUIMainWindow* pMainWindow)
: QAbstractTableModel()
{
	m_pMainWindow=pMainWindow;
}
#pragma endregion

#pragma region "DeConstructor"
CCarrierModel::~CCarrierModel()
{

}
#pragma endregion

#pragma region "int CCarrierModel::rowCount(const QModelIndex & ) const "
int CCarrierModel::rowCount(const QModelIndex & ) const 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;
	int count=m_pMainWindow->m_pProject->m_CarrierList.count();
	return count;
}
#pragma endregion

#pragma region "int CCarrierModel::columnCount(const QModelIndex & ) const "
int CCarrierModel::columnCount(const QModelIndex & ) const 
{
	return 8;
}
#pragma endregion

#pragma region "void CCarrierModel::RefreshList()"
void CCarrierModel::RefreshList()
{
	endResetModel();
}
#pragma endregion

#pragma region "QVariant CCarrierModel::data(const QModelIndex &index, int role) const "
QVariant CCarrierModel::data(const QModelIndex &index, int role) const 
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
			return (QVariant) ( Qt::AlignRight  | Qt::AlignVCenter );
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

		if(row<0 || row>=m_pMainWindow->m_pProject->m_CarrierList.count())
			return QVariant();

		__Carrier* rowData=m_pMainWindow->m_pProject->m_CarrierList.at(row);
		QString tmp;
		switch(col)
		{
		case 0:
			return rowData->id;
		case 1:
			return rowData->name;
		case 2:
			return rowData->Depth;
		case 3:
			return rowData->Width;
		case 4:
			return rowData->Height;
		case 5:
			return rowData->MaxLoad;
		case 6:
			return rowData->CostPerKm;
		case 7:
			return rowData->VahicleType;
		}
	}
	return QVariant();
}
#pragma endregion

#pragma region "bool CCarrierModel::setData(const QModelIndex &index, const QVariant &value, int role) "
bool CCarrierModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;

	if (index.isValid() && role == Qt::EditRole)
	{
		// we have index.row() index.column() and value
		// all we need
		int col=index.column();
		int row=index.row();

		if(row<0 || row>=m_pMainWindow->m_pProject->m_CarrierList.count())
			return false;

		__Carrier* rowData=m_pMainWindow->m_pProject->m_CarrierList.at(row);

		if(col==0)
		{
			int id=value.toInt();
			rowData->id=id;
		}
		else if(col==1)
		{
			QString name=value.toString();
			rowData->name=name;
		}
		else if(col==2)
		{
			bool bOk;
			int Length=value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->Depth=Length;
		}
		else if(col==3)
		{
			bool bOk;
			int Width=value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->Width=Width;
		}
		else if(col==4)
		{
			bool bOk;
			int Height=value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->Height=Height;
		}
		else if(col==5)
		{
			bool bOk;
			int MaxLoad=value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->MaxLoad=MaxLoad;
		}
		else if(col==6)
		{
			bool bOk;
			int CostPerKm=value.toLongLong(&bOk);
			if(!bOk)
				return false;
			rowData->CostPerKm=CostPerKm;
		}
		else if(col==7)
		{
			bool bOk;
			int VahicleType=value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->VahicleType=VahicleType;
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}
#pragma endregion

#pragma region "Qt::ItemFlags CCarrierModel::flags(const QModelIndex &index) const "
Qt::ItemFlags CCarrierModel::flags(const QModelIndex &index) const 
{
	if (!index.isValid()) return Qt::ItemIsEnabled;
	int col=index.column();
	int row=index.row();
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}
#pragma endregion

#pragma region "QVariant CCarrierModel::headerData(int section, Qt::Orientation orientation, int role) const  "
QVariant CCarrierModel::headerData(int section, Qt::Orientation orientation, int role) const  
{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation == Qt::Horizontal) 
	{
		switch(section)
		{
		case 0:
			return QString("Code");//"Code"
		case 1:
			return QString("name");//"name"
		case 2:
			return QString("Length(cm)");//"Length(cm)"
		case 3:
			return QString("Width(cm)");//"Width(cm)"
		case 4:
		    return QString("height(cm)");//"height(cm)"
		case 5:
			return QString("MaxLoad");//"MaxLoad"
		case 6:
			return QString("cost per km");//"cost per km"
		case 7:
			return QString("Vehicle Type");//"Vehicle Type"
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

