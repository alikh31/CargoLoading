#ifndef ABSTRACT_ITEM_DELEGATE__H
#define ABSTRACT_ITEM_DELEGATE__H

#include <QItemDelegate>
#include <QFontMetrics>
#include <QModelIndex>
#include <QSize>
#include <QAbstractItemModel>
#include <QPainter>
#include <QList>

/// Brief description. 
///CAbstractItemDelegate Class is a QItemDelegate base class for Displaying the content of a specified item ...



/*! \detail
this class has a QList of all items it must keep them alligned ,the most important virtual function which is used in this class 
is the sizeHint Function 
*/

class CAbstractItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
	/*! 
	Constructor Only accepts parent pointer as the input , if this parametrer was NULL it uses the MainFrame pointer as the parent
	*/
					CAbstractItemDelegate   (QObject *parent = 0);
public:
	/*! 
	the class uses this function to paint the background of the item ,in the case that m_bDrawBk is true and the index of item is equal to 
	m_iBkRow and m_iBkColumn.the background color is always yellow.
	*/
    void			paint				    (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	/*! 
	the class uses this function to adjust the size of the items
	*/
    virtual QSize   sizeHint				(const QStyleOptionViewItem &option,const QModelIndex &index ) const;
	/*! 
	this function adds the value w to the list of class widths .
	*/
	void			AddColumnWidth			(int w);
	/*! 
	this variable holds the value of the column height in pixel .by increasing this value we will have cells with bigger heights.
	*/
	int				m_ColumnH;
	/*! 
	this variable holds the values of the column widths.
	*/
	QList<int>      m_listColumnWidth		;
	/*! 
	this variable holds the background status of the item.
	*/
	bool			m_bDrawBk;
	/*! 
	this variable holds row index of the item which should have the yellow background color .
	*/
	int				m_iBkRow;
	/*! 
	this variable holds column index of the item which should have the yellow background color .
	*/
	int				m_iBkColumn;
};
#endif
