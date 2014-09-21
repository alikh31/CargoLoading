#include "ProviderModel.h"
#include "GUIMainWindow.h"



CProviderModel::CProviderModel(CGUIMainWindow* pMainWindow)
: QAbstractTableModel()
{
	m_pMainWindow=pMainWindow;
}

CProviderModel::~CProviderModel()
{

}

int CProviderModel::rowCount(const QModelIndex & ) const 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;
	int count=m_pMainWindow->m_pProject->m_ProviderList.count();
	return count;
}

int CProviderModel::columnCount(const QModelIndex & ) const 
{
	return 5;
}

void CProviderModel::RefreshList()
{
	endResetModel();
}

QVariant CProviderModel::data(const QModelIndex &index, int role) const 
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

		if(row<0 || row>=m_pMainWindow->m_pProject->m_ProviderList.count())
			return QVariant();

		__Provider* rowData=m_pMainWindow->m_pProject->m_ProviderList.at(row);
		QString tmp;
		switch(col)
		{
		case 0:
			return rowData->id;
		case 1:
			return rowData->name;
		case 2:
			return rowData->Address;
		case 3:
			return rowData->phi;
		case 4:
			return rowData->landa;
		}
	}
	return QVariant();
}

bool CProviderModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;

	if (index.isValid() && role == Qt::EditRole)
	{
		// we have index.row() index.column() and value
		// all we need
		int col=index.column();
		int row=index.row();

		if(row<0 || row>=m_pMainWindow->m_pProject->m_ProviderList.count())
			return false;

		__Provider* rowData=m_pMainWindow->m_pProject->m_ProviderList.at(row);

		if(col==0)
		{
			int id =value.toInt();
			rowData->id=id ;
		}
		else if(col==1)
		{
			QString name =value.toString();
			rowData->name=name;
		}
		else if(col==2)
		{
			QString address=value.toString();
			rowData->Address=address;
		}
		else if(col==3)
		{
			bool bOk;
			double phi =value.toDouble(&bOk);
			if(!bOk)
				return false;
			rowData->phi=phi;
		}
		else if(col==4)
		{
			bool bOk;
			double Landa =value.toDouble(&bOk);
			if(!bOk)
				return false;
			rowData->landa=Landa;
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

Qt::ItemFlags CProviderModel::flags(const QModelIndex &index) const 
{
	if (!index.isValid()) return Qt::ItemIsEnabled;
	int col=index.column();
	int row=index.row();
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}

QVariant CProviderModel::headerData(int section, Qt::Orientation orientation, int role) const  
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
			return QString("address");//"address"
		case 3:
			return QString("Location X(meter)");//"Location X(meter)"
		case 4:
			return QString("Location Y(meter)");//"Location Y(meter)"
		}
	}
	else 
	{
		QString headerName=QString("%1").arg(section);
		return headerName;

	}
	return "";
}

