#ifndef PROVIDERWIDGET_H
#define PROVIDERWIDGET_H

#include <QDockWidget>
#include "ProviderTable.h"
#include "GUIMainWindow.h"
#include <QPushButton>

class CProviderWidget : public QDockWidget
{
	Q_OBJECT

public:
	CProviderWidget(QString title , QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CProviderWidget();
public:
	CProviderTable* m_pTable;
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

#endif // CARGOWIDGET_H
