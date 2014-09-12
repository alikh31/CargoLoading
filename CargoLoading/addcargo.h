#ifndef ADDCARGO_H
#define ADDCARGO_H

#include <QDialog>
#include "ui_addcargo.h"
#include "GUIMainWindow.h"

using namespace Ui;

class AddCargo : public QDialog, public AddCargoClass
{
	Q_OBJECT

public:
	AddCargo(QWidget *parent = 0,CGUIMainWindow* pMainWindow=NULL);
	~AddCargo();

public:
	CGUIMainWindow* m_pMainWindow;
	bool isAccepted;

public slots:
    void onClickedAccept();
    void onClickedReject();

};

#endif // ADDCARGO_H
