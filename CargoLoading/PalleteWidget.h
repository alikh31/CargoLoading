#ifndef PALLETEWIDGET_H
#define PALLETEWIDGET_H

#include <QDockWidget>
#include "PalleteTable.h"
#include "GUIMainWindow.h"
#include <QPushButton>

class CPalleteWidget : public QDockWidget
{
	Q_OBJECT

public:
	CPalleteWidget(QString title , QWidget *parent=NULL,CGUIMainWindow* pMainWindow=NULL);
	~CPalleteWidget();
public:
	CPalleteTable				* m_pTable;
	QPushButton					*m_pNewRow;
	QPushButton					*m_pClearAll;
	QPushButton					*m_pDistances;
public:
	void SetupButton(QString Text,QPushButton* pButton,QHBoxLayout* pLayout);
public slots:
	void OnAddNewRow   (bool bOk);
	void OnClearAllRows(bool bOk);


};

#endif // CARRIERWIDGET_H