#pragma once
#include <qlist.h>
#include <QStringList.h>
#include <math.h>
#include "Project.h"
#include "OptimazedPacker.h"

#ifndef __PATH_OPTIMIZER_H
#define __PATH_OPTIMIZER_H

#define PI 3.14159265
#define EarthRadius 6376.5

class CPathOptimizer
{
public:
	CPathOptimizer(QList<__Carrier*>CarrierList, QList<__Order*> OrderList , QList<__BoxRow*> CargoList ,QList<__Provider*> ProviderList,double phi,double landa  , bool modeFlag = 0);
	~CPathOptimizer(void);

public:
	QList<__BoxRow*>	m_CargoList;
	QList<__Order*>		m_OrderList;
	QList<__Provider*>	m_ProviderList;


	QList<Node*>		m_UnassignedNode;

	QList<Path*>		m_AllPathes;

	QList<__Carrier*>m_CarrierList;
	__Carrier* m_Carrier;

	bool m_fMode;

	int m_minTime ;
	int m_maxTime ;

	double m_phi;
	double m_landa;

public:
	bool		Compute				(QString & OutputMessage);
	void		Optimization		();
	void		fillAllPath			();
	void		OptimizeCarrier		();
	void		ReArange			();

	void		checkOverLoad		(Node* node);

	__Carrier*	putInContainer		(__Carrier* Carrier,Node* node);
	__Carrier*	putAllInContainer	(__Carrier* Carrier,QList<Node*> node);
	__Provider* GetProvider			(int id);

	__Carrier*	GetBigestCarrier			();


	bool		isOverLoad			(__Carrier* Carrier,Node* node);
	bool		checkTimeLimit		(Node* node);
	bool		isWorthChange		(Path* path1 ,Node* node1 ,Path* path2 ,Node* node2);
	bool		isWorthReArange		(Path* path ,Node* node1 ,Node* node2);

	bool		isAbleToRemove		(Path* path1 ,Path* path2 ,Node* node2);
	bool		replaceNode			(Path* path ,Node* node = NULL );
	bool		MaxOptimization		(Path* path ,Node* node = NULL );
	bool		isAbleToChangeCarrier(Path* path1, __Carrier* Carrier);

	Node*		findMinDist			();

	double		CalculateDistanse	(double x1,double y1,double x2,double y2);
	//double CalculateDistanse(double phi1,double landa1,double phi2,double landa2);
	double		CalculateDistanse	(Node *node1,Node *node2);

	void		CalculatePenaltyCost(__Carrier* carrier);
};


#endif
