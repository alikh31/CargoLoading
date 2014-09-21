#include "PalleteModel.h"
#include "GUIMainWindow.h"



CPalleteModel::CPalleteModel(CGUIMainWindow* pMainWindow)
: QAbstractTableModel()
{
	m_pMainWindow=pMainWindow;
}

CPalleteModel::~CPalleteModel()
{

}

int CPalleteModel::rowCount(const QModelIndex & ) const 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;
	int count=m_pMainWindow->m_pProject->m_PalleteList.count();
	return count;
}

int CPalleteModel::columnCount(const QModelIndex & ) const 
{
	return m_pMainWindow->m_pProject->m_PalleteList.count()+2;
}

void CPalleteModel::RefreshList()
{
	endResetModel();
}

QVariant CPalleteModel::data(const QModelIndex &index, int role) const 
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

		if(row<0 || row>=m_pMainWindow->m_pProject->m_PalleteList.count())
			return QVariant();

		__Pallete* rowData=m_pMainWindow->m_pProject->m_PalleteList.at(row);
		QString tmp;
		switch(col)
		{
		case 0:
			return rowData->id;
		case 1:
			return rowData->name;
		}
		return rowData->bIsPutOver[col-2];
	}
	return QVariant();
}

bool CPalleteModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
	if(!m_pMainWindow || !m_pMainWindow->m_pProject)
		return 0;

	if (index.isValid() && role == Qt::EditRole)
	{
		// we have index.row() index.column() and value
		// all we need
		int col=index.column();
		int row=index.row();

		if(row<0 || row>=m_pMainWindow->m_pProject->m_PalleteList.count())
			return false;

		__Pallete* rowData=m_pMainWindow->m_pProject->m_PalleteList.at(row);

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
		else 
		{
			bool bOk;
			bOk=value.toBool();
			rowData->bIsPutOver[col-2]=bOk;
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

Qt::ItemFlags CPalleteModel::flags(const QModelIndex &index) const 
{
	if (!index.isValid()) return Qt::ItemIsEnabled;
	int col=index.column();
	int row=index.row();
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}

QVariant CPalleteModel::headerData(int section, Qt::Orientation orientation, int role) const  
{
	if (role != Qt::DisplayRole) return QVariant();
	if (orientation == Qt::Horizontal) 
	{
		switch(section)
		{
		case 0:
			return QString("Code");//"Code"
		case 1:
			return QString("Name");//"Name"
		}
		__Pallete* rowData=m_pMainWindow->m_pProject->m_PalleteList.at(section-2);
		QString tmp;
		tmp=QString("On(%1)").arg(rowData->name);
		return tmp;
	}
	else 
	{
		QString headerName=QString("%1").arg(section);
		return headerName;

	}
	return "";
}

