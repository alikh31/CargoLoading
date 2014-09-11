#ifndef CARRIERWIDGET_H
#define CARRIERWIDGET_H

#include <QDockWidget>
#include "CarrierTable.h"
#include "GUIMainWindow.h"
#include <QPushButton>

class CCarrierWidget : public QDockWidget
{
	Q_OBJECT

public:
	CCarrierWidget(QString title , QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CCarrierWidget();
public:
	CCarrierTable				* m_pTable;
	QPushButton					*m_pNewRow;
	QPushButton					*m_pClearAll;
	QPushButton					*m_pDistances;
	CGUIMainWindow				*m_pMainWindow;
public:
	void SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout);
public slots:
	void OnAddNewRow   (bool bOk);
	void OnClearAllRows(bool bOk);


};

#endif // CARRIERWIDGET_H