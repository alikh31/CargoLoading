#ifndef __PROJECT__HH
#define __PROJECT__HH

#include "rows.h"
#include "PathOptimizer.h"
#include <qstring>


class CPathOptimizer;
	
class CProject
{
public:
	CProject(void);
	~CProject(void);
public:
	void		AddNewCargo			();
	void		AddNewProvider		();
	void		AddNewCarrier		();
	void		AddNewPallete		();
	void		AddNewOrder			();
public:
	void		ClearAll			();
	void		newProject			();
	void		closeProject		();
	bool		Open				(QString fileName);
	void		Save				();
	void		SaveAs				(QString fileName);
public:
	__BoxRow* GetBox				(int id);
	__BoxRow* GetBoxByName			(QString name);

	__Provider* GetProvider			(int id);
	__Provider* GetProviderByName	(QString name);

public:
	QList<__BoxRow*>				m_CargoList;
	QList<__Provider*>				m_ProviderList;
	QList<__Carrier*>				m_CarrierList;
	QList<__Pallete*>				m_PalleteList;
	QList<__Order*>					m_OrderList;
	QString						    m_filePath;
	CPathOptimizer*					m_pCPathOptimizer;
	bool							m_fMode;

	QList<Path*>					m_AllPathes;
public:
	CBuffer						   *m_pBuff;
public:
	void		Compute			   ();
	void		ComputeByTSPack    ();
	
	
};
#endif