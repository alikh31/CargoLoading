#ifndef ADDCARRIER_H
#define ADDCARRIER_H

#include <QDialog>
#include <QMessageBox>
#include "ui_addcarrier.h"
#include "GUIMainWindow.h"


class Addcarrier : public QDialog , public Ui_addcarrierclass
{
	Q_OBJECT

public:
	Addcarrier(QWidget *parent = 0 ,CGUIMainWindow* pMainWindow=NULL);
	~Addcarrier();

public:
	CGUIMainWindow* m_pMainWindow;
	bool isAccepted;

	public slots:
		void onClickedAccept();

};

#endif // ADDCARRIER_H
