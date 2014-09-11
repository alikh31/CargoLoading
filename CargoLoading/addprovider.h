#ifndef ADDPROVIDER_H
#define ADDPROVIDER_H

#include <QDialog>
#include <QMessageBox>
#include "ui_addprovider.h"
#include "GUIMainWindow.h"
class dss
{
public:
	dss(int a=0,int b=0)
	{
		m_a = a;
		m_b = b;
	};
	int mmax()
	{
		if(m_b<m_a)
			return m_a;
		return m_b;

	};
	int m_a;
	int m_b;

};

class AddProvider : public QDialog , public Ui_addProviderclass
{
	Q_OBJECT

public:
	AddProvider(QWidget *parent = 0 ,CGUIMainWindow* pMainWindow=NULL);
	~AddProvider();

public:
	CGUIMainWindow* m_pMainWindow;
	bool isAccepted;

public slots:
	void onClickedAccept();

};

#endif // ADDProvider_H
