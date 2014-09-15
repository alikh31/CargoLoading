#pragma region "Includes"
#include "CargoModel.h"
#include "GUIMainWindow.h"


#pragma endregion

#pragma region "Constructor"
CCargoModel::CCargoModel(CGUIMainWindow* pMainWindow)
	: QAbstractTableModel()
{
	m_pMainWindow=pMainWindow;
}
#pragma endregion

#pragma region "DeConstructor"
CCargoModel::~CCargoModel()
{

}
#pragma endregion

#pragma region "int CCargoModel::rowCount(const QModelIndex & ) const "
int CCargoModel::rowCount(const QModelIndex & ) const 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;
	int count=m_pMainWindow->m_pProject->m_CargoList.count();
	return count;
}
#pragma endregion

#pragma region "int CCargoModel::columnCount(const QModelIndex & ) const "
int CCargoModel::columnCount(const QModelIndex & ) const 
{
	return 9;
}
#pragma endregion

#pragma region "void CCargoModel::RefreshList()"
void CCargoModel::RefreshList()
{
    endResetModel();
}
#pragma endregion

#pragma region "QVariant CCargoModel::data(const QModelIndex &index, int role) const "
QVariant CCargoModel::data(const QModelIndex &index, int role) const 
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

		if(row<0 || row>=m_pMainWindow->m_pProject->m_CargoList.count())
			return QVariant();

		__BoxRow* rowData=m_pMainWindow->m_pProject->m_CargoList.at(row);
		QString tmp;
		switch(col)
		{
		case 0:
			return rowData->id;
		case 1:
			return rowData->Desc;
		case 2:
			return rowData->PaleteSizeD;
		case 3:
			return rowData->PaleteSizeW;
		case 4:
			return rowData->PaleteSizeH;
		case 5:
			return rowData->PaleteWeight;
		case 6:
			return rowData->FloorIndex;
		case 7:
			return rowData->MaxLoad;
		case 8:
			return rowData->Color;
		}
	}
	return QVariant();
}
#pragma endregion

#pragma region "bool CCargoModel::setData(const QModelIndex &index, const QVariant &value, int role) "
bool CCargoModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;

	if (index.isValid() && role == Qt::EditRole)
	{
		// we have index.row() index.column() and value
		// all we need
		int col=index.column();
		int row=index.row();

		if(row<0 || row>=m_pMainWindow->m_pProject->m_CargoList.count())
			return false;
		
		__BoxRow* rowData=m_pMainWindow->m_pProject->m_CargoList.at(row);

		if(col==0)
		{
			QString Code =value.toString();
			rowData->Code=Code;
		}
		else if(col==1)
		{
			QString Desc =value.toString();
			rowData->Desc=Desc;
		}
		else if(col==2)
		{
			bool bOk;
			int PaleteSizeL =value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->PaleteSizeD=PaleteSizeL;
		}
		else if(col==3)
		{
			bool bOk;
			int PaleteSizeW =value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->PaleteSizeW=PaleteSizeW;
		}
		else if(col==4)
		{
			bool bOk;
			int PaleteSizeH =value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->PaleteSizeH=PaleteSizeH;
		}
		else if(col==5)
		{
			bool bOk;
			double PaleteWeight =value.toDouble(&bOk);
			if(!bOk)
				return false;
			rowData->PaleteWeight=PaleteWeight;
		}
		else if(col==6)
		{
			QString st=value.toString();
			QString  FloorIndex =st;
			rowData->FloorIndex=FloorIndex;
		}
		/*else if(col==8)
		{
			
			bool Priority  =value.toBool();
			rowData->Priority =Priority ;
		}*/
		else if(col==7)
		{
			bool bOk;
			int MaxLoad =value.toInt(&bOk);
			if(!bOk)
				return false;
			rowData->MaxLoad =MaxLoad ;
		}
		else if(col==8)
		{
			bool bOk;
			QString Color =value.toString();
			rowData->Color = Color;
		}

		emit dataChanged(index, index);
		return true;
	}
	return false;
}
#pragma endregion

#pragma region "Qt::ItemFlags CCargoModel::flags(const QModelIndex &index) const "
Qt::ItemFlags CCargoModel::flags(const QModelIndex &index) const 
{
	if (!index.isValid()) return Qt::ItemIsEnabled;
	int col=index.column();
	int row=index.row();
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
	
}
#pragma endregion

#pragma region "QVariant CCargoModel::headerData(int section, Qt::Orientation orientation, int role) const  "
QVariant CCargoModel::headerData(int section, Qt::Orientation orientation, int role) const  
{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation == Qt::Horizontal) 
	{
		switch(section)
		{
		case 0:
			return QString("Code");//"Code"
		case 1:
			return QString("Description");//"Description"	
		//case 2:
		//	return QString("Type");//"Type"
		case 2:
			return QString("Length(cm)");//"Length(cm)"
		case 3:
			return QString("Width(cm)");//"Width(cm)"
		case 4:
			return QString("height(cm)");//"height(cm)"
		case 5:
			return QString("Weight(kg)");//"Weight(kg)"
		case 6:
			return QString("Floor Index");//"Floor Index"
		//case 8:
		//	return QString("Priority");//"Priority"
		case 7:
			return QString("MaxLoad");//"MaxLoad"
		case 8:
			return QString("Color");//"Color"

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

