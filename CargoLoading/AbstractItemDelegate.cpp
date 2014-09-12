#pragma region "Includes"
#include <QtGui>
#include "AbstractItemDelegate.h"
int				m_maximumWidth;
#pragma endregion



#pragma region "Constructor"
CAbstractItemDelegate::CAbstractItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
	m_ColumnH=20;
	m_bDrawBk=false;
	m_iBkRow=0;
	m_iBkColumn=1;
	m_maximumWidth=0;
}
#pragma endregion

#pragma region "Paint"
void CAbstractItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
	if(m_bDrawBk && index.row()==m_iBkRow && index.column()==m_iBkColumn)
		painter->fillRect(option.rect,QBrush(Qt::yellow));

	return QItemDelegate::paint(painter, option,index);
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    //int size = qMin(option.rect.width(), option.rect.height());
    QString str = index.model()->data(index, Qt::DisplayRole).toString();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    //painter->setPen(Qt::Soli);
    
	if (option.state & QStyle::State_Selected)
        painter->setBrush(option.palette.highlightedText());
    else
        painter->setBrush(QBrush(Qt::black));
	painter->drawText(option.rect.x(), option.rect.y()+10, str);
    //painter->drawEllipse(QRectF(option.rect.x() + option.rect.width()/2 - radius,
    //                            option.rect.y() + option.rect.height()/2 - radius,
    //                            2*radius, 2*radius));
    painter->restore();
}
#pragma endregion

#pragma region "sizeHint"
QSize CAbstractItemDelegate::sizeHint(const QStyleOptionViewItem &  option , const QModelIndex &  index ) const
{
	int col=index.column();
	if(col>=0 && col<m_listColumnWidth.count())
	{
		QSize retSize=QItemDelegate::sizeHint(option , index ) ;
		int parentWidth=retSize.width();
		int maxWidth=parentWidth>m_listColumnWidth.at(col) ? parentWidth :m_listColumnWidth.at(col);
		return QSize(maxWidth,m_ColumnH);
		return QSize(m_listColumnWidth.at(col),m_ColumnH);
	}
	else
		//return QSize(m_ColumnH,m_ColumnH);
	{
		QSize retSize=QItemDelegate::sizeHint(option , index ) ;
		if(retSize.width()>m_maximumWidth)
			m_maximumWidth=retSize.width();
		return QSize(m_maximumWidth,m_ColumnH);
	}
}
#pragma endregion

#pragma region "void CAbstractItemDelegate::AddColumnWidth(int w)"
void CAbstractItemDelegate::AddColumnWidth(int w)
{
	m_listColumnWidth.append(w);
}
#pragma endregion