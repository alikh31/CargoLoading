#include <QList>
#include <QSize>
#include <QString>
#include "Buffer.h"
#include <math.h>

struct __BoxRow
{
	__BoxRow()
	{
		id=BoxCounter;
		NodeId = 0;
		PaleteSizeD=0;
		PaleteSizeW=0;
		PaleteSizeH=0;
		PaleteWeight=0;
		FloorIndex="1 2 3 4 5 6";
		Color= "1 1 1";
		Priority =false;
		MaxLoad=100;
		BoxCounter++;
	}
	int id;
	QString Code;
	QString Desc;

	int NodeId;

	int     PaleteSizeD;//cm
	int     PaleteSizeW;//cm
	int     PaleteSizeH;//cm

	double  PaleteWeight;//kg
	QString		FloorIndex;//1 to 6 
	QString		Color;

	bool    Priority ;
	int     MaxLoad;//kg
    static int BoxCounter;

	void SaveToBuffer(CBuffer* pBuffer)
	{
		pBuffer->AddTail(id);
		pBuffer->AddTail(Code);
		pBuffer->AddTail(Desc);
		pBuffer->AddTail(PaleteSizeD);
		pBuffer->AddTail(PaleteSizeW);
		pBuffer->AddTail(PaleteSizeH);
		pBuffer->AddTail(PaleteWeight);
		pBuffer->AddTail(FloorIndex);
		pBuffer->AddTail(Color);
		int tmp=(Priority==true ? 1 :0);
		pBuffer->AddTail(tmp);
		pBuffer->AddTail(MaxLoad);
	}

	void ReadFromBuffer(CBuffer* pBuffer)
	{
		id=pBuffer->GetHeadI();
		Code=pBuffer->GetHeadST();
		Desc=pBuffer->GetHeadST();
		PaleteSizeD=pBuffer->GetHeadI();
		PaleteSizeW=pBuffer->GetHeadI();
		PaleteSizeH=pBuffer->GetHeadI();
		PaleteWeight=pBuffer->GetHeadD();
		FloorIndex=pBuffer->GetHeadST();
		Color=pBuffer->GetHeadST();
		int tmp=pBuffer->GetHeadI();
		Priority=(tmp==1 ? true :false);
		MaxLoad=pBuffer->GetHeadI();
	}
};

struct __BoxInContainer
{
	__BoxInContainer()
	{
		w = 0;
		h = 0;
		d = 0;

		x = 0;
		y = 0;
		z = 0;

		NodeId = 0;

		ContainerID = 0;
	}

	__BoxInContainer(const __BoxInContainer& box)
	{
		w = box.w;
		h = box.h;
		d = box.d;

		x = box.x;
		y = box.y;
		z = box.z;

		NodeId = box.NodeId;

		ContainerID = box.ContainerID;
	}

	int w;
	int h;
	int d;

	int x;
	int y;
	int z;

	int NodeId;

	int ContainerID;
};

struct __Provider
{
	__Provider()
	{
		id=ProviderCounter;
		phi=0;
		landa=0;
		ProviderCounter++;
	}

	__Provider(const __Provider& provider)
	{
		id=provider.id;
		phi=provider.phi;
		landa=provider.landa;
		name = provider.name;
		Address = provider.Address;
	}


	int		id;
	QString name;
	QString Address;
	double phi;
	double landa;
	static int ProviderCounter;
	void SaveToBuffer(CBuffer* pBuffer)
	{
		pBuffer->AddTail(id);
		pBuffer->AddTail(name);
		pBuffer->AddTail(Address);
		pBuffer->AddTail(phi);
		pBuffer->AddTail(landa);
	}

	void ReadFromBuffer(CBuffer* pBuffer)
	{
		id=pBuffer->GetHeadI();
		name=pBuffer->GetHeadST();
		Address=pBuffer->GetHeadST();
		phi=pBuffer->GetHeadD();
		landa=pBuffer->GetHeadD();
	}
};

class __CArea
{
public:
	__CArea(double W = 0,double H = 0,double X = 0,double Y = 0,double FloorZ = 0,double TopZ = 0,int maxLoad = 0)
	{
		m_X = X;
		m_Y = Y;
		m_FloorZ = FloorZ;
		m_TopZ = TopZ;
		m_W = W;
		m_H = H;
		m_MaxLoad = maxLoad;
	};

	__CArea(const __CArea& area)
	{
		m_X = area.m_X;
		m_Y = area.m_Y;
		m_FloorZ = area.m_FloorZ;
		m_TopZ = area.m_TopZ;
		m_W = area.m_W;
		m_H = area.m_H;
		m_MaxLoad = area.m_MaxLoad;
	};

	~__CArea()
	{

	};
	double m_W;
	double m_H;
	double m_X;
	double m_Y;
	double m_FloorZ;
	double m_TopZ;

	int m_MaxLoad;

	bool isEqual(__CArea* pClosest)
	{
		if (m_FloorZ == pClosest->m_FloorZ && m_H == pClosest->m_H && m_W == pClosest->m_W
			&& m_X == pClosest->m_X && m_Y == pClosest->m_Y && m_TopZ == pClosest->m_TopZ)
			return true;
		return false;
	}
};

class __Carrier
{
public:
	__Carrier()
	{
		id=CarrierCounter;
		Depth=0;
		Width=0;
		Height=0;
		MaxLoad=0;
		CostPerKm=0;
		VahicleType=0;
		CarrierCounter++;
	}
	__Carrier(const __Carrier& carrier)
	{
		id=carrier.id;
		name = carrier.name;
		Depth=carrier.Depth;
		Width=carrier.Width;
		Height=carrier.Height;
		MaxLoad=carrier.MaxLoad;
		CostPerKm=carrier.CostPerKm;
		VahicleType=carrier.VahicleType;

		m_FloorAreas.clear();
		for(int i = 0;i<carrier.m_FloorAreas.count(); i++)
		{
			__CArea* TEMP = new __CArea(*carrier.m_FloorAreas.at(i));
			m_FloorAreas.append(TEMP);
		}

		m_TopAreas.clear();
		for(int i = 0;i<carrier.m_TopAreas.count(); i++)
		{
			__CArea* TEMP = new __CArea(*carrier.m_TopAreas.at(i));
			m_TopAreas.append(TEMP);
		}

		m_BoxList.clear();
		for(int i = 0;i<carrier.m_BoxList.count(); i++)
		{
			__BoxInContainer* TEMP = new __BoxInContainer(*carrier.m_BoxList.at(i));
			m_BoxList.append(TEMP);
		}
	}
	~__Carrier()
	{
		while (!m_FloorAreas.isEmpty())
		{
			__CArea* TEMP = m_FloorAreas.takeFirst();
			if(TEMP!=NULL)
			{
				delete TEMP;
				TEMP = NULL;
			}
		}
		while (!m_TopAreas.isEmpty())
		{
			__CArea* TEMP = m_TopAreas.takeFirst();
			if(TEMP!=NULL)
			{
				delete TEMP;
				TEMP = NULL;
			}
		}

		while(!m_BoxList.isEmpty())
		{	
			__BoxInContainer* temp = m_BoxList.takeFirst();
			if(temp != NULL)
			{
				delete temp;
				temp = NULL;
			}
		}
	}
public:
	int		id;
	QString name;
	int Depth;//cm
	int Width;//cm
	int Height;//cm
	int MaxLoad;//kg
	long CostPerKm;//$
	int VahicleType;
	QList<__CArea*> m_FloorAreas;
	QList<__CArea*> m_TopAreas;
	static int CarrierCounter;
	QList<__BoxInContainer*> m_BoxList;
	void SaveToBuffer(CBuffer* pBuffer)
	{
		pBuffer->AddTail(id);
		pBuffer->AddTail(name);
		pBuffer->AddTail(Depth);
		pBuffer->AddTail(Width);
		pBuffer->AddTail(Height);
		pBuffer->AddTail(MaxLoad);
		pBuffer->AddTail(CostPerKm);
		pBuffer->AddTail(VahicleType);
	}
	void ReadFromBuffer(CBuffer* pBuffer)
	{
		id=pBuffer->GetHeadI();
		name=pBuffer->GetHeadST();
		Depth=pBuffer->GetHeadI();
		Width=pBuffer->GetHeadI();
		Height=pBuffer->GetHeadI();
		MaxLoad=pBuffer->GetHeadI();
		CostPerKm=pBuffer->GetHeadL();
		VahicleType=pBuffer->GetHeadI();
	}
};

struct __Pallete
{
	__Pallete()
	{
		id=PalleteCounter;
		for(int i=0;i<100;i++)
		{
			bIsPutOver[i]=true;
		}
		PalleteCounter++;
	}
	int id;
	QString name;
	bool    bIsPutOver[100];
	static int PalleteCounter;
	void SaveToBuffer(CBuffer* pBuffer)
	{
		pBuffer->AddTail(id);
		pBuffer->AddTail(name);
		for(int i=0;i<100;i++)
		{
			int tmp=(bIsPutOver[i]==true ? 1 :0);
			pBuffer->AddTail(tmp);
		}
	}
	void ReadFromBuffer(CBuffer* pBuffer)
	{
		id=pBuffer->GetHeadI();
		name=pBuffer->GetHeadST();
		for(int i=0;i<100;i++)
		{
			int tmp=pBuffer->GetHeadI();
			bIsPutOver[i]=(tmp==1 ? true :false);
		}
	}
};

struct __Order
{
	__Order()
	{
		id=OrderCounter;
		BoxID=-1;
		ProviderID=-1;
		count=0;
		minTime=0;
		maxTime=0;
		OrderCounter++;
		//m_BoxList=NULL;
	}
	__Order(const __Order& order)
	{
		 id = order.id;
		 BoxID= order.BoxID;
		 count= order.count;
		 minTime= order.minTime;
		 maxTime= order.maxTime;
		 distFromCenter= order.distFromCenter;
		 ProviderID= order.ProviderID;
		 pProvider= new __Provider(*order.pProvider);
		
	}
	int id;
	int BoxID;
	int count;
	int minTime;
	int maxTime;
	double distFromCenter;
	int ProviderID;
	__Provider* pProvider;
	static int OrderCounter;
	void PrepareBoxList()
	{
		/*if(m_BoxList)
		{
			delete [] m_BoxList;
			m_BoxList=NULL;
		}
		m_BoxList=new __BoxInContainer[count];*/
	}
	void SaveToBuffer(CBuffer* pBuffer)
	{
		pBuffer->AddTail(id);
		pBuffer->AddTail(BoxID);
		pBuffer->AddTail(count);
		pBuffer->AddTail(minTime);
		pBuffer->AddTail(maxTime); 
		pBuffer->AddTail(ProviderID);
	}
	void ReadFromBuffer(CBuffer* pBuffer)
	{
		id=pBuffer->GetHeadI();
		BoxID=pBuffer->GetHeadI();
		count=pBuffer->GetHeadI();
		minTime=pBuffer->GetHeadI();
		maxTime=pBuffer->GetHeadI();
		ProviderID=pBuffer->GetHeadI();
	}
};

class Node
{
public:
	Node()
	{
		id = NodeCounter;

		m_Dist = 0;
		m_phi = 0;
		m_landa = 0;

		NodeCounter++;
	}

	Node(const Node& node)
	{
		id = node.id;
		m_Dist = node.m_Dist;
		m_landa = node.m_landa;
		m_phi = node.m_phi;
		m_Order = new __Order(*node.m_Order);
	}

	~Node()
	{
		/*delete m_Order;
		m_Order = NULL;*/
	}
	int id;

	double m_Dist;
	double m_phi;
	double m_landa;

	__Order  *m_Order;

	static int NodeCounter;

	bool operator==(Node temp)
	{
		if(m_Dist== temp.m_Dist &&
			m_phi== temp.m_phi &&
			m_landa == temp.m_landa &&
			m_Order == temp.m_Order)
			return true;
		return false;
	}
};

class Path
{
public:
	Path()
	{
		id = PathCounter;

		m_Cost = 0;
		m_PenaltyCost = 0;

		m_minTime = 0;
		m_maxTime = 0;
		
		PathCounter++;
	}

	Path(const Path& path)
	{
		id = path.id;

		m_Cost = path.m_Cost;
		m_PenaltyCost = path.m_PenaltyCost;

		m_minTime = path.m_minTime ;
		m_maxTime = path.m_maxTime ;

		//PathCounter++;

		for(int i = 0;i<path.m_Node.count(); i++)
		{	
			Node* temp = new Node (*path.m_Node.at(i));
			m_Node.append(temp);
		}

		m_Carrier = new __Carrier(*path.m_Carrier);


	}


	~Path()
	{
		while(!m_Node.isEmpty())
		{	
			Node* temp = m_Node.takeFirst();
			if(temp != NULL)
			{
				delete temp;
				temp = NULL;
			}
		}

		if(m_Carrier != NULL)
		{
			delete m_Carrier;
			m_Carrier = NULL;
		}
	}

	void copyFrom(Path path)
	{
		id = path.id;

		m_Cost = path.m_Cost;
		m_PenaltyCost = path.m_PenaltyCost;

		m_minTime = path.m_minTime ;
		m_maxTime = path.m_maxTime ;

		//PathCounter++;

		for(int i = 0;i<path.m_Node.count(); i++)
		{	
			Node* tNode = path.m_Node.at(i);

			Node* temp = new Node();

			*temp = *tNode;

			temp->m_Order = new __Order();

			*temp->m_Order = *tNode->m_Order;

			m_Node.append(temp);
		}

		m_Carrier = new __Carrier();
		*m_Carrier = *path.m_Carrier;

		for(int i = 0;i<path.m_Carrier->m_FloorAreas.count(); i++)
		{
			__CArea* TEMP = new __CArea();
			*TEMP = *path.m_Carrier->m_FloorAreas.at(i);
			m_Carrier->m_FloorAreas.append(TEMP);
		}

		for(int i = 0;i<path.m_Carrier->m_TopAreas.count(); i++)
		{
			__CArea* TEMP = new __CArea();
			*TEMP = *path.m_Carrier->m_TopAreas.at(i);
			m_Carrier->m_TopAreas.append(TEMP);
		}

		for(int i = 0;i<path.m_Carrier->m_BoxList.count(); i++)
		{
			__BoxInContainer* TEMP = new __BoxInContainer();
			*TEMP = *path.m_Carrier->m_BoxList.at(i);
			m_Carrier->m_BoxList.append(TEMP);
		}

		return;

	}

	int id;

	QList<Node*> m_Node;

	__Carrier* m_Carrier;

	int m_minTime ;
	int m_maxTime ;

	int m_Cost;
	double m_PenaltyCost;

	static int PathCounter;

	void CalculateCost()
	{
		double SumDist = 0;
		Node *Temp;
		for(int j=0;j<m_Node.count();j++)
		{
			Temp = m_Node.at(j);
			SumDist += (double)Temp->m_Dist;
		}
		SumDist  += pow(((double)Temp->m_phi-0)*((double)Temp->m_phi-0)+((double)Temp->m_landa-0)*((double)Temp->m_landa-0),0.5);
		m_Cost  = (double)SumDist * (double)m_Carrier->CostPerKm;
	}

	void CalculatePenaltyCost()
	{
		double TotalArea = 0;
		for(int i = 0 ; i < m_Carrier->m_FloorAreas.count() ; i++)
		{
			__CArea* Temp = m_Carrier->m_FloorAreas.at(i);
			TotalArea += (double)Temp->m_H * (double)Temp->m_W *((double)Temp->m_TopZ-(double)Temp->m_FloorZ);
		}

		for(int i = 0 ; i < m_Carrier->m_TopAreas.count() ; i++)
		{
			__CArea* Temp = m_Carrier->m_TopAreas.at(i);
			TotalArea += (double)Temp->m_H * (double)Temp->m_W *((double)Temp->m_TopZ-(double)Temp->m_FloorZ);
		}

		double SumDist = 0;
		for(int j=0;j<m_Node.count();j++)
		{
			Node *Temp = m_Node.at(j);
			SumDist += (double)Temp->m_Dist;
		}

		m_PenaltyCost = (((double)TotalArea) * (double)m_Carrier->CostPerKm * (double)SumDist);
		//m_PenaltyCost = ((TotalArea) * m_Carrier->CostPerKm * SumDist)/1000;
	}

};
