#include "PathOptimizer.h"

#pragma region"CPathOptimizer::CPathOptimizer(QList<__Carrier*>CarrierList, QList<__Order*> OrderList , QList<__BoxRow*> CargoList)"
CPathOptimizer::CPathOptimizer(QList<__Carrier*>CarrierList, QList<__Order*> OrderList , QList<__BoxRow*> CargoList ,QList<__Provider*> ProviderList,double phi,double landa , bool modeFlag)
{
	m_CarrierList = CarrierList;
	m_Carrier = GetBigestCarrier();
	m_CargoList = CargoList;
	m_OrderList = OrderList;
	m_ProviderList = ProviderList;

	m_phi = phi;
	m_landa = landa;

	m_fMode = modeFlag;
}
#pragma endregion

#pragma region"COptimazedPacker::~COptimazedPacker(void)"
CPathOptimizer::~CPathOptimizer(void)
{
}
#pragma endregion

#pragma region"bool CPathOptimizer::Compute(QString & OutputMessage)"
bool CPathOptimizer::Compute(QString & OutputMessage)
{
	m_minTime = 0;
	m_maxTime = 0;

	__Carrier* Carrier = new __Carrier(*m_Carrier);
	Carrier->m_TopAreas.clear();

	Node* Center = new Node();
	Center->m_Dist = 0;
	Center->m_phi = m_phi;
	Center->m_landa = m_landa;
	Center->m_Order = new __Order();

	while (m_OrderList.count() != 0)
	{
		__Order* Temp = m_OrderList.takeFirst();
		Temp->pProvider = GetProvider(Temp->ProviderID);
		if(Temp->count != 0 && Temp->pProvider)
		{
			Node* m_Node = new Node();
			m_Node->m_phi = Temp->pProvider->phi;
			m_Node->m_landa = Temp->pProvider->landa;
			m_Node->m_Dist = CalculateDistanse(Center , m_Node);
			m_Node->m_Order = Temp;

			checkOverLoad(m_Node);
		}
	}

	Path* CorrentPath = new Path();
	while (m_UnassignedNode.count() != 0)
	{
		for (int i = 0; i < m_UnassignedNode.count(); i++)
		{
			Node *Temp = m_UnassignedNode.at(i);
			Temp->m_Dist = CalculateDistanse(Center , Temp);
		}


		Node *Corrent = findMinDist();


		

		if(!isOverLoad(Carrier,Corrent))
		{
			if(!checkTimeLimit(Corrent))
			{
				for(int i = 0; i < m_UnassignedNode.count() ; i++)
				{
					Node* TEMP = m_UnassignedNode.at(i);
					if(TEMP == Corrent)
					{
						m_UnassignedNode.removeAt(i);
					}
				}
				m_OrderList.append(Corrent->m_Order);
				continue;
			}
			//__Carrier* TEMP;
			Carrier = putInContainer(Carrier,Corrent);
			CorrentPath->m_Carrier = new __Carrier(*Carrier);
			Center = Corrent;
			for(int i = 0; i < m_UnassignedNode.count() ; i++)
			{
				Node* TEMP = m_UnassignedNode.at(i);
				if(TEMP == Corrent)
				{
					m_UnassignedNode.removeAt(i);
				}
			}
			CorrentPath->m_Node.append(Corrent);
		}
		else
		{
			for(int i = 0; i < m_UnassignedNode.count() ; i++)
			{
				Node* TEMP = m_UnassignedNode.at(i);
				if(TEMP == Corrent)
				{
					m_UnassignedNode.removeAt(i);
				}
			}
			m_OrderList.append(Corrent->m_Order);
			continue;
		}
	}

	if(CorrentPath->m_Node.count()!=0)
	{
		CorrentPath->m_minTime = m_minTime;
		CorrentPath->m_maxTime = m_maxTime;

		CorrentPath->CalculateCost();
		CorrentPath->CalculatePenaltyCost();

		m_AllPathes.append(CorrentPath);
	}

	if(m_OrderList.count() != 0)
		Compute(OutputMessage);

	if(Carrier != NULL)
	{
		delete Carrier;
		Carrier = NULL;
	}

	return false;
}
#pragma endregion

#pragma region"__Carrier CPathOptimizer::putInContainer(__Carrier Carrier,Node* node)"
__Carrier* CPathOptimizer::putInContainer(__Carrier* Carrier,Node* node)
{
	__Carrier* pCarrier = new __Carrier(*Carrier);

	if(pCarrier->m_FloorAreas.count() == 0 && pCarrier->m_TopAreas.count() == 0)
	{
		__CArea* pTotalFloor = new __CArea(pCarrier->Width,pCarrier->Depth,0,0,0,pCarrier->Height , pCarrier->MaxLoad);
		pCarrier->m_FloorAreas.append(pTotalFloor);
	}

	Node* pNode = new Node(*node);

	QList<Node*> pNodeList;
	pNodeList.append(pNode);


	COptimazedPacker *pOptimazedPacker = new COptimazedPacker(pCarrier, pNodeList , m_CargoList);

	QString mString;

	if(pOptimazedPacker->Compute(mString))
	{
		Carrier = new __Carrier(*pCarrier);

		Carrier->m_FloorAreas  = pOptimazedPacker->m_ALLFloorAreas;

		Carrier->m_TopAreas = pOptimazedPacker->m_ALLTopAreas;

		if(pOptimazedPacker!=NULL)
		{
			delete pOptimazedPacker;
			pOptimazedPacker = NULL;
		}

		if(pNode != NULL)
		{
			delete pNode;
			pNode = NULL;
		}

		return Carrier;
	}

	if(pOptimazedPacker!=NULL)
	{
		delete pOptimazedPacker;
		pOptimazedPacker = NULL;
	}

	if(pNode != NULL)
	{
		delete pNode;
		pNode = NULL;
	}

	return Carrier;
}
#pragma endregion

#pragma region"__Carrier CPathOptimizer::putAllInContainer(__Carrier* Carrier,QList<Node*> node)"
__Carrier* CPathOptimizer::putAllInContainer(__Carrier* Carrier,QList<Node*> node)
{
	__Carrier* pCarrier = new __Carrier(*Carrier);
	pCarrier->m_BoxList.clear();
	pCarrier->m_FloorAreas.clear();
	pCarrier->m_TopAreas.clear();

	if(pCarrier->m_FloorAreas.count() == 0 && pCarrier->m_TopAreas.count() == 0)
	{
		__CArea* pTotalFloor = new __CArea(pCarrier->Width,pCarrier->Depth,0,0,0,pCarrier->Height , pCarrier->MaxLoad);
		pCarrier->m_FloorAreas.append(pTotalFloor);
	}

	QList<Node*> pNodeList;

	for(int i = 0 ; i < node.count() ; i++)
	{
		Node* pNode = new Node(*node.at(i));

		pNodeList.append(pNode);
	}


	COptimazedPacker *pOptimazedPacker = new COptimazedPacker(pCarrier, pNodeList , m_CargoList);

	QString mString;

	if(pOptimazedPacker->Compute(mString))
	{
		Carrier = new __Carrier(*pCarrier);

		Carrier->m_FloorAreas  = pOptimazedPacker->m_ALLFloorAreas;

		Carrier->m_TopAreas = pOptimazedPacker->m_ALLTopAreas;

		if(pOptimazedPacker!=NULL)
		{
			delete pOptimazedPacker;
			pOptimazedPacker = NULL;
		}

		while (! pNodeList.isEmpty())
		{
			Node* tNode = pNodeList.takeFirst();
			if(tNode != NULL)
			{
				delete tNode;
				tNode = NULL;
			}
		}

		return Carrier;
	}

	if(pOptimazedPacker!=NULL)
	{
		delete pOptimazedPacker;
		pOptimazedPacker = NULL;
	}
	while (! pNodeList.isEmpty())
	{
		Node* tNode = pNodeList.takeFirst();
		if(tNode != NULL)
		{
			delete tNode;
			tNode = NULL;
		}
	}

	Carrier = NULL;

	return Carrier;
}
#pragma endregion

#pragma region"bool CPathOptimizer::checkTimeLimit(Node node)"
bool CPathOptimizer::checkTimeLimit(Node *node)
{
	int minTime = node->m_Order->minTime;
	int maxTime = node->m_Order->maxTime;

	if(m_minTime == 0 && m_maxTime == 0)
	{
		m_minTime = minTime;
		m_maxTime = maxTime;
		return true;
	}

	if(minTime >= m_minTime && maxTime <= m_maxTime)
	{
		m_maxTime = maxTime;
		m_minTime = minTime;
		return true;
	}

	if(minTime <= m_minTime && maxTime >= m_maxTime)
	{
		return true;
	}

	if(minTime > m_maxTime || maxTime < m_minTime)
	{
		return false;
	}

	if(minTime <= m_minTime && maxTime <= m_maxTime)
	{
		m_maxTime = maxTime;
		return true;
	}

	if(minTime >= m_minTime && maxTime >= m_maxTime)
	{
		m_minTime = minTime;
		return true;
	}

	return false;
}
#pragma endregion

#pragma region"bool CPathOptimizer::isOverLoad(__Carrier Carrier,Node* node)"
bool CPathOptimizer::isOverLoad(__Carrier* Carrier,Node* node)
{
	__Carrier* pCarrier = new __Carrier(*Carrier);

	if(pCarrier->m_FloorAreas.count() == 0 && pCarrier->m_TopAreas.count() == 0)
	{
		__CArea* pTotalFloor = new __CArea(pCarrier->Width,pCarrier->Depth,0,0,0,pCarrier->Height , pCarrier->MaxLoad);
		pCarrier->m_FloorAreas.append(pTotalFloor);
	}

	
	Node* pNode = new Node(*node);

	QList<Node*> pNodeList;
	pNodeList.append(pNode);


	COptimazedPacker *pOptimazedPacker = new COptimazedPacker(pCarrier, pNodeList , m_CargoList);

	QString mString;

	if(pOptimazedPacker->Compute(mString))
	{
		//delete pCarrier;
		if(pOptimazedPacker!=NULL)
		{
			delete pOptimazedPacker;
			pOptimazedPacker = NULL;
		}
		if(pCarrier != NULL)
		{
			delete pCarrier ;
			Carrier  = NULL;
		}
		if(pNode != NULL)
		{
			delete pNode ;
			pNode = NULL;
		}
		return false;
	}
	if(pOptimazedPacker!=NULL)
	{
		delete pOptimazedPacker;
		pOptimazedPacker = NULL;
	}
	if(pCarrier != NULL)
	{
		delete pCarrier ;
		Carrier  = NULL;
	}
	if(pNode != NULL)
	{
		delete pNode ;
		pNode = NULL;
	}
	return true;
}
#pragma endregion

#pragma region"void CPathOptimizer::checkOverLoad(Node* node)"
void CPathOptimizer::checkOverLoad(Node* node)
{
	__Carrier* pCarrier = new __Carrier(*m_Carrier);

	if(pCarrier->m_FloorAreas.count() == 0 && pCarrier->m_TopAreas.count() == 0)
	{
		__CArea* pTotalFloor = new __CArea(pCarrier->Width,pCarrier->Depth,0,0,0,pCarrier->Height , pCarrier->MaxLoad);
		pCarrier->m_FloorAreas.append(pTotalFloor);
	}

	Node* pNode = new Node(*node);

	QList<Node*> pNodeList;
	pNodeList.append(pNode);

	
	COptimazedPacker *pOptimazedPacker = new COptimazedPacker(pCarrier, pNodeList , m_CargoList);

	QString mString;


	if(pOptimazedPacker->Compute(mString))
	{
		m_UnassignedNode.append(node);
		//delete pCarrier;
		if(pOptimazedPacker!=NULL)
		{
			delete pOptimazedPacker;
			pOptimazedPacker = NULL;
		}
		return;
	}

	if(pOptimazedPacker->m_Carrier->m_BoxList.count()==0)
	{
		//delete pCarrier;
		if(pOptimazedPacker!=NULL)
		{
			delete pOptimazedPacker;
			pOptimazedPacker = NULL;
		}
		return;
	}

	Node *assignedNode = new Node(*node);
	assignedNode->m_Order->count = pOptimazedPacker->m_Carrier->m_BoxList.count();
	m_UnassignedNode.append(assignedNode);


	Node *TEMP = new Node(*node);
	TEMP->m_Order->count = pOptimazedPacker->m_UnAssignedBoxes.count();
	
	//delete pCarrier;
	if(pOptimazedPacker!=NULL)
	{
		delete pOptimazedPacker;
		pOptimazedPacker = NULL;
	}
	checkOverLoad(TEMP);
}
#pragma endregion

#pragma region"Node* CPathOptimizer::findMinDist()"
Node* CPathOptimizer::findMinDist()
{
	double min = 1.79769e+308;

	for(int i = 0; i < m_UnassignedNode.count() ; i++)
	{
		Node *Temp  = m_UnassignedNode.at(i);
		if(Temp->m_Dist<min)
			min = Temp->m_Dist;
	}
	for(int i = 0; i < m_UnassignedNode.count() ; i++)
	{
		Node* Temp  = m_UnassignedNode.at(i);
		if(Temp->m_Dist == min)
			return Temp;
	}
	return NULL;
}
#pragma endregion

#pragma region"double CPathOptimizer::CalculateDistanse(double phi1,double landa1,double phi2,double landa2)"
//double CPathOptimizer::CalculateDistanse(double phi1,double landa1,double phi2,double landa2)
//{
//	double dDistance = 0;
//	double dLat1InRad = phi1 * (PI / 180.0);
//	double dLong1InRad = landa1 * (PI / 180.0);
//	double dLat2InRad = phi2 * (PI / 180.0);
//	double dLong2InRad = landa2 * (PI / 180.0);
//
//	double dLongitude = dLong2InRad - dLong1InRad;
//	double dLatitude = dLat2InRad - dLat1InRad;
//
//	// Intermediate result a.
//	double a = pow(sin(dLatitude / 2.0), 2.0) + 
//		cos(dLat1InRad) * cos(dLat2InRad) * 
//		pow(sin(dLongitude / 2.0), 2.0);
//
//	// Intermediate result c (great circle distance in Radians).
//	double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
//
//	// Distance.
//	// const Double kEarthRadiusMiles = 3956.0;
//	dDistance = EarthRadius * c;
//
//	return dDistance;
//}
#pragma endregion

#pragma region"double CPathOptimizer::CalculateDistanse(double x1,double y1,double x2,double y2)"
double CPathOptimizer::CalculateDistanse(double x1,double y1,double x2,double y2)
{
	return pow((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2),0.5);
}
#pragma endregion

#pragma region"double CPathOptimizer::CalculateDistanse(Node node1,Node node2)"
double CPathOptimizer::CalculateDistanse(Node* node1,Node* node2)
{
	return CalculateDistanse(node1->m_phi,node1->m_landa,node2->m_phi,node2->m_landa);
}
#pragma endregion

#pragma region "__Provider* CPathOptimizer::GetProvider(int id)"
__Provider* CPathOptimizer::GetProvider(int id)
{
	for(int i=0;i<m_ProviderList.count();i++)
	{
		__Provider* pro=m_ProviderList.at(i);
		if(pro->id==id)
			return pro;
	}
	return NULL;
}
#pragma endregion

#pragma region "void CPathOptimizer::Optimization()"
void CPathOptimizer::Optimization()
{
	for(int i = 0 ; i < m_AllPathes.count() ; i++)
	{
		Path* CorrentPath = m_AllPathes.at(i);


		for(int j = 0 ; j < CorrentPath->m_Node.count() ; j++)
		{

			for(int k = i+1 ; k < m_AllPathes.count() ; k++)
			{
				Path* TempPath = m_AllPathes.at(k);

				for(int t = 0 ; t < TempPath->m_Node.count() ; t++)
				{
					Path* path1 = new Path(*CorrentPath);

					Path* path2 = new Path(*TempPath);

					Node* CorrentNode = path1->m_Node.at(j);

					Node* TempNode = path2->m_Node.at(t);

					if(isWorthChange(path1 ,CorrentNode ,path2,TempNode))
					{
						if(CorrentPath !=NULL)
						{
							delete CorrentPath ;
							CorrentPath = NULL;
						}		
						CorrentPath = new Path(*path1);

						m_AllPathes.replace(i , CorrentPath);				
						

						if(TempPath !=NULL)
						{
							delete TempPath ;
							TempPath = NULL;
						}
						TempPath = new Path(*path2);
						m_AllPathes.replace(k , TempPath);				
						
						Optimization();
					}
					
					if(path1!=NULL)
					{
						delete path1;
						path1 = NULL;
					}
					if(path2!=NULL)
					{
						delete path2;
						path2 = NULL;
					}
					
				}
			}
		}

		

	}
}
#pragma endregion

#pragma region "bool CPathOptimizer::isWorthChange(Path* path1 ,Node* node1 ,Path* path2 ,Node* node2)"
bool CPathOptimizer::isWorthChange(Path* path1 ,Node* node1 ,Path* path2 ,Node* node2)
{
	double firstPenaltyCost1 = path1->m_PenaltyCost;
	double firstPenaltyCost2 = path2->m_PenaltyCost;

	path1->m_Node.removeOne(node1);
	path2->m_Node.removeOne(node2);

	if(m_fMode)
	{
		if(!replaceNode(path1,node2))
			return false;

		if(!replaceNode(path2,node1))
			return false;
	}
	else
	{
		if(!MaxOptimization(path1,node2))
			return false;

		if(!MaxOptimization(path2,node1))
			return false;
	}

	if(firstPenaltyCost1<=path1->m_PenaltyCost)
		return false;

	if(firstPenaltyCost2<=path2->m_PenaltyCost)
		return false;

	return true;
}
#pragma endregion

#pragma region "bool CPathOptimizer::isWorthReArange(Path* path1 ,Node* node1 ,Node* node2)"
bool CPathOptimizer::isWorthReArange(Path* path ,Node* node1 ,Node* node2)
{
	double firstPenaltyCost1 = path->m_PenaltyCost;

	__Carrier* Carrier = new __Carrier(*path->m_Carrier);
	Carrier->m_TopAreas.clear();
	Carrier->m_FloorAreas.clear();
	Carrier->m_BoxList.clear();


	Node* Center = new Node();
	Center->m_Dist = 0;
	Center->m_phi = m_phi;
	Center->m_landa = m_landa;
	Center->m_Order = new __Order();


	m_minTime = 0;
	m_maxTime = 0;

	m_UnassignedNode.clear();

	while(!path->m_Node.isEmpty())
	{
		Node* Temp = path->m_Node.takeFirst();
		if(Temp->id == node1->id)
			m_UnassignedNode.append(node2);
		else if(Temp->id == node2->id)
			m_UnassignedNode.append(node1);
		else
			m_UnassignedNode.append(Temp);
	}

	if (m_UnassignedNode.count() == 0)
	{
		path->m_Carrier = Carrier;
	}

	while(!m_UnassignedNode.isEmpty())
	{
		Node *Corrent = m_UnassignedNode.takeFirst();

		Corrent->m_Dist = CalculateDistanse(Center , Corrent);

		if(!isOverLoad(Carrier,Corrent))
		{
			if(!checkTimeLimit(Corrent))
			{
				return false;
			}
			Carrier = putInContainer(Carrier,Corrent);;
			Center = Corrent;
			for(int i = 0; i < m_UnassignedNode.count() ; i++)
			{
				Node* TEMP = m_UnassignedNode.at(i);
				if(TEMP == Corrent)
				{
					m_UnassignedNode.removeAt(i);
				}
			}
			path->m_Node.append(Corrent);
		}
		else
		{
			return false;
		}
	}


	if(path->m_Node.count()!=0)
	{
		path->m_minTime = m_minTime;
		path->m_maxTime = m_maxTime;

		path->CalculateCost();
		path->CalculatePenaltyCost();
	}



	if(firstPenaltyCost1<=path->m_PenaltyCost)
		return false;

	return true;
}
#pragma endregion

#pragma region"bool CPathOptimizer::replaceNode(Path* path ,Node* node )"
bool CPathOptimizer::replaceNode(Path* path ,Node* node )
{
	__Carrier* Carrier = new __Carrier(*m_Carrier);
	Carrier->m_TopAreas.clear();
	Carrier->m_FloorAreas.clear();
	Carrier->m_BoxList.clear();


	Node* Center = new Node();
	Center->m_Dist = 0;
	Center->m_phi = m_phi;
	Center->m_landa = m_landa;
	Center->m_Order = new __Order();


	m_minTime = 0;
	m_maxTime = 0;

	m_UnassignedNode.clear();

	if(node != NULL)
		m_UnassignedNode.append(node);
	while(!path->m_Node.isEmpty())
	{
		Node* Temp = path->m_Node.takeFirst();
		m_UnassignedNode.append(Temp);
	}

	if (m_UnassignedNode.count() == 0)
	{
		path->m_Carrier = Carrier;
	}

	while (m_UnassignedNode.count() != 0)
	{
		for (int i = 0; i < m_UnassignedNode.count(); i++)
		{
			Node *Temp = m_UnassignedNode.at(i);
			Temp->m_Dist = CalculateDistanse(Center , Temp);
		}


		Node *Corrent = findMinDist();


		

		if(!isOverLoad(Carrier,Corrent))
		{
			if(!checkTimeLimit(Corrent))
			{
				return false;
			}
			//__Carrier* TEMP;
			//TEMP = putInContainer(Carrier,Corrent);

			__Carrier *TCar =  putInContainer(Carrier,Corrent);

			if(TCar  == NULL)
				return false;

			Carrier = new __Carrier(*TCar);

			//path->m_Carrier = putInContainer(Carrier,Corrent);;

			Center = Corrent;
			for(int i = 0; i < m_UnassignedNode.count() ; i++)
			{
				Node* TEMP = m_UnassignedNode.at(i);
				if(TEMP == Corrent)
				{
					m_UnassignedNode.removeAt(i);
				}
			}
			path->m_Node.append(Corrent);
		}
		else
		{
			return false;
		}
	}

	path->m_Carrier = new __Carrier(*Carrier);

	if(path->m_Node.count()!=0)
	{
		path->m_minTime = m_minTime;
		path->m_maxTime = m_maxTime;

		path->CalculateCost();
		path->CalculatePenaltyCost();
	}

	//Center = new Node();
	//Center->m_Dist = 0;
	//Center->m_phi = m_phi;
	//Center->m_landa = m_landa;
	//Center->m_Order = new __Order();


	//for(int j = 0 ; j < path->m_Node.count() ; j++)
	//{
	//	Node* CorrentNode = path->m_Node.at(j);

	//	for(int t = j+1 ; t < path->m_Node.count() ; t++)
	//	{
	//		Node* TempNode = path->m_Node.at(t);

	//		Path* path1 = new Path(*path);
	//		//*path1 = *path;

	//		if(isWorthReArange(path1 ,CorrentNode,TempNode))
	//		{
	//			if(path!=NULL)
	//			{
	//				delete path;
	//				path = NULL;
	//			}
	//			path = new Path(*path1);
	//		}
	//		else
	//		{
	//			if(path1!=NULL)
	//			{
	//				delete path1;
	//				path1 = NULL;
	//			}
	//		}
	//	}
	//}

	return true;
}
#pragma endregion

#pragma region"bool CPathOptimizer::MaxOptimization(Path* path ,Node* node )"
bool CPathOptimizer::MaxOptimization(Path* path ,Node* node )
{
	__Carrier* Carrier = new __Carrier(*m_Carrier);
	Carrier->m_TopAreas.clear();
	Carrier->m_FloorAreas.clear();
	Carrier->m_BoxList.clear();

	Node* Center = new Node();
	Center->m_Dist = 0;
	Center->m_phi = m_phi;
	Center->m_landa = m_landa;
	Center->m_Order = new __Order();


	m_minTime = 0;
	m_maxTime = 0;

	m_UnassignedNode.clear();

	if(node != NULL)
		m_UnassignedNode.append(node);
	while(!path->m_Node.isEmpty())
	{
		Node* Temp = path->m_Node.takeFirst();
		m_UnassignedNode.append(Temp);
	}

	if (m_UnassignedNode.count() == 0)
	{
		path->m_Carrier = new __Carrier(*Carrier);
	}

	QList<Node*> allNodeList;

	while (m_UnassignedNode.count() != 0)
	{
		for (int i = 0; i < m_UnassignedNode.count(); i++)
		{
			Node *Temp = m_UnassignedNode.at(i);
			Temp->m_Dist = CalculateDistanse(Center , Temp);
		}


		Node *Corrent = findMinDist();

		


		if(!isOverLoad(Carrier,Corrent))
		{
			if(!checkTimeLimit(Corrent))
			{
				return false;
			}
			//TEMP = putInContainer(Carrier,Corrent);
			allNodeList.append(Corrent);
			Center = Corrent;
			for(int i = 0; i < m_UnassignedNode.count() ; i++)
			{
				Node* TEMP = m_UnassignedNode.at(i);
				if(TEMP == Corrent)
				{
					m_UnassignedNode.removeAt(i);
				}
			}
			path->m_Node.append(Corrent);
		}
		else
		{
			return false;
		}
	}


	__Carrier *TCar = putAllInContainer(Carrier  , allNodeList);

	if(TCar  == NULL)
		return false;


	if(path->m_Carrier != NULL)
	{
		delete path->m_Carrier ;
		path->m_Carrier  = NULL;
	}
	path->m_Carrier = TCar;

	if(path->m_Node.count()!=0)
	{
		path->m_minTime = m_minTime;
		path->m_maxTime = m_maxTime;

		path->CalculateCost();
		path->CalculatePenaltyCost();
	}

	if(Carrier!= NULL)
	{	
		delete Carrier;
		Carrier = NULL;
	}
	
	
	return true;
}
#pragma endregion

#pragma region"void CPathOptimizer::fillAllPath()"
void CPathOptimizer::fillAllPath()
{
	for(int i = 0 ; i < m_AllPathes.count() ; i++)
	{
		Path* CorrentPath = m_AllPathes.at(i);

		for(int k = i+1 ; k < m_AllPathes.count() ; k++)
		{
			Path* TempPath = m_AllPathes.at(k);

			for(int t = 0 ; t < TempPath->m_Node.count() ; t++)
			{
				Path* path1 = new Path(*CorrentPath);

				Path* path2 = new Path(*TempPath);

				Node* TempNode = path2->m_Node.at(t);

				if(isAbleToRemove(path1 ,path2,TempNode))
				{
					if(CorrentPath !=NULL)
					{
						delete CorrentPath ;
						CorrentPath = NULL;
					}		
					CorrentPath = new Path(*path1);

					m_AllPathes.replace(i , CorrentPath);				


					if(TempPath !=NULL)
					{
						delete TempPath ;
						TempPath = NULL;
					}
					TempPath = new Path(*path2);
					m_AllPathes.replace(k , TempPath);		
				}
				if(path1!=NULL)
				{
					delete path1;
					path1 = NULL;
				}
				if(path2!=NULL)
				{
					delete path2;
					path2 = NULL;
				}
			}
		}
		if(CorrentPath->m_Node.count() == 0)
		{
			Path* TPath = m_AllPathes.takeAt(i);				
			if(TPath!=NULL)
			{
				delete TPath;
				TPath = NULL;
			}
		}
		
	}
}
#pragma endregion

#pragma region"void CPathOptimizer::ReArange()"
void CPathOptimizer::ReArange()
{
	for(int i = 0 ; i < m_AllPathes.count() ; i++)
	{
		Path* CorrentPath = m_AllPathes.at(i);
		m_Carrier = CorrentPath->m_Carrier;

		MaxOptimization(CorrentPath);
	}

	for(int i = 0 ; i < m_AllPathes.count() ; i++)
	{
		Path* CorrentPath = m_AllPathes.at(i);

		for(int j = 0 ; j < CorrentPath->m_Node.count() ; j++)
		{
			Node* CorrentNode = CorrentPath->m_Node.at(j);

			for(int t = j+1 ; t < CorrentPath->m_Node.count() ; t++)
			{
				Node* TempNode = CorrentPath->m_Node.at(t);

				Path* path1 = new Path(*CorrentPath);
				//copyPath(path1,CorrentPath);
				//*path1 = *CorrentPath;

				if(isWorthReArange(path1 ,CorrentNode,TempNode))
				{
					m_AllPathes.replace(i , path1);				
					
					if(CorrentPath!=NULL)
					{
						//delete CorrentPath;
						//CorrentPath = NULL;
					}
				}
				else
				{
					if(path1!=NULL)
					{
						//delete path1;
						//path1 = NULL;
					}
				}
			}
		}

	}
}
#pragma endregion

#pragma region "bool CPathOptimizer::isAbleToRemove(Path* path1 ,Path* path2 ,Node* node2)"
bool CPathOptimizer::isAbleToRemove(Path* path1 ,Path* path2 ,Node* node2)
{
	double firstPenaltyCost1 = path1->m_PenaltyCost;
	double firstPenaltyCost2 = path2->m_PenaltyCost;

	path2->m_Node.removeOne(node2);

	if(m_fMode)
	{
		if(!replaceNode(path1,node2))
			return false;

		replaceNode(path2 , NULL);
	}
	else
	{
		if(!MaxOptimization(path1,node2))
			return false;

		MaxOptimization(path2 , NULL);
	}

	return true;
}
#pragma endregion

#pragma region "__Carrier*	CPathOptimizer::GetBigestCarrier()"
__Carrier*	CPathOptimizer::GetBigestCarrier()
{
	double v = 0;
	for(int i = 0 ; i < m_CarrierList.count() ; i++)
	{
		__Carrier* Carrier=(__Carrier*)m_CarrierList.at(i);
		double t = 0;
		t = (double)Carrier->Height * (double)Carrier->Depth * (double)Carrier->Width;
		if(t > v)
		{
			v = t;
		}
	}
	
	for(int i = 0 ; i < m_CarrierList.count() ; i++)
	{
		__Carrier* Carrier=(__Carrier*)m_CarrierList.at(i);
		double t = 0;
		t = (double)Carrier->Height * (double)Carrier->Depth * (double)Carrier->Width;
		if(t == v)
		{
			return Carrier;
		}
	}

	return NULL;
}
#pragma endregion

#pragma region "void CPathOptimizer::OptimizeCarrier()"
void CPathOptimizer::OptimizeCarrier()
{
	for(int j= 0 ; j < m_CarrierList.count() ; j++)
	{
		m_Carrier = new __Carrier(*m_CarrierList.at(j));
		m_Carrier->m_TopAreas.clear();
		m_Carrier->m_FloorAreas.clear();
		m_Carrier->m_BoxList.clear();


		for(int i = 0 ; i < m_AllPathes.count() ; i++)
		{
			Path* CorrentPath = m_AllPathes.at(i);

			Path* path1 = new Path(*CorrentPath);
			//*path1 = *CorrentPath;

			if(isAbleToChangeCarrier(path1 ,m_Carrier))
			{
				m_AllPathes.replace(i,path1);
				if(CorrentPath!=NULL)
				{
					delete CorrentPath;
					CorrentPath = NULL;
				}
			}
			else
			{
				if(path1!=NULL)
				{
					delete path1;
					path1 = NULL;
				}
			}
		}
	}
	
}
#pragma endregion

#pragma region "bool CPathOptimizer::isAbleToChangeCarrier(Path* path1 ,__Carrier* Carrier)"
bool CPathOptimizer::isAbleToChangeCarrier(Path* path1 ,__Carrier* Carrier)
{
	double firstPenaltyCost1 = path1->m_PenaltyCost;

	m_Carrier = new __Carrier(*Carrier);

	if(m_fMode)
	{
		if(!replaceNode(path1))
			return false;
	}
	else
	{
		if(!MaxOptimization(path1))
			return false;
	}
	
	if(firstPenaltyCost1>path1->m_PenaltyCost)
		return true;

	return false;
}
#pragma endregion