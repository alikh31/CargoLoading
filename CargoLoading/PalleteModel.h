#ifndef PALLETEMODEL_H
#define PALLETEMODEL_H

#include <QAbstractTableModel>

class CGUIMainWindow;
class CPalleteModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CPalleteModel(CGUIMainWindow* pMainWindow);
	~CPalleteModel();
public:
	int			  rowCount			(const QModelIndex &parent = QModelIndex())			 const;
	int			  columnCount		(const QModelIndex & )								 const;
	QVariant	  headerData		(int section, Qt::Orientation orientation, int role) const;
	QVariant	  data				(const QModelIndex &index, int role)				 const;
	Qt::ItemFlags flags				(const QModelIndex &index)							 const;
	bool		  setData			(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	void		  RefreshList		();


public:
	CGUIMainWindow* m_pMainWindow;


};

#endif // CARRIERMODEL_H