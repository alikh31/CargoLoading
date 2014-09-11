#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QDockWidget>
#include "OrderTable.h"
#include "GUIMainWindow.h"
#include <QPushButton>


class COrderWidget : public QDockWidget
{
	Q_OBJECT

public:
	COrderWidget(QString title , QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~COrderWidget();
public:
	COrderTable					*m_pTable;
	QPushButton					*m_pNewRow;
	QPushButton					*m_pClearAll;
	QPushButton					*m_pDistances;
	QPushButton					*m_pCompute;
	CGUIMainWindow				*m_pMainWindow;

public:
	void SetupButton			(QString Text,QPushButton* pButton,QHBoxLayout* pLayout);
public slots:
	void OnAddNewRow			(bool bOk);
	void OnClearAllRows			(bool bOk);
	void OnCompute				(bool bOk);


};

#endif // CARRIERWIDGET_H