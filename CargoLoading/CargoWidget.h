#ifndef CARGOWIDGET_H
#define CARGOWIDGET_H

#include <QDockWidget>
#include "CargoTable.h"
#include "GUIMainWindow.h"
#include <QPushButton>

class CCargoWidget : public QDockWidget
{
	Q_OBJECT

public:
	CCargoWidget(QString title , QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CCargoWidget();
public:
	CGUIMainWindow* m_pMainWindow;
	CCargoTable* m_pTable;
	QPushButton					*m_pNewRow;
	QPushButton					*m_pClearAll;
	QPushButton					*m_pDistances;
public:
	void SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout);
	
public slots:
	void OnAddNewRow   (bool bOk);
	void OnClearAllRows(bool bOk);

	
};

#endif // CARGOWIDGET_H
