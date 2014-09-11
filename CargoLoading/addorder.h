#ifndef ADDORDER_H
#define ADDORDER_H

#include <QDialog>
#include <QMessageBox>
#include "ui_addorder.h"
#include "GUIMainWindow.h"


class AddOrder : public QDialog , public Ui_AddOrderClass
{
	Q_OBJECT

public:
	AddOrder(QWidget *parent = 0 ,CGUIMainWindow* pMainWindow=NULL);
	~AddOrder();

public:
	CGUIMainWindow* m_pMainWindow;
	bool isAccepted;

public slots:
	void onClickedAccept();

};

#endif // ADDORDER_H
