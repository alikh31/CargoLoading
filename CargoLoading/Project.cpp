#pragma region "Includes"
#include "Project.h"
#include <QFile>
#include <QByteArray>
#include "QCoreApplication"


int __BoxRow::BoxCounter=0;
int __Provider::ProviderCounter=0;
int __Carrier::CarrierCounter=0;
int __Pallete::PalleteCounter=0;
int __Order::OrderCounter=0;
int Path::PathCounter=1;
int Node::NodeCounter=1;
#pragma endregion

#pragma region "CProject::CProject(void)"
CProject::CProject(void)
{
	m_fMode = false;
	m_pBuff=new CBuffer();

	m_filePath = QCoreApplication::applicationDirPath();
	m_filePath += "/DATA.dat";

	//m_filePath = fileName;

	ClearAll();
	QFile file(m_filePath);
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray bArray;
		bArray=file.readAll();
		char* ch=bArray.data();
		if(!m_pBuff)
			m_pBuff=new CBuffer();
		m_pBuff->SetBuffer(ch,bArray.length());

		int CargoCount =m_pBuff->GetHeadI();
		for(int i=0;i<CargoCount;i++)
		{
			__BoxRow* pBox=new __BoxRow();
			pBox->ReadFromBuffer(m_pBuff);
			m_CargoList.append(pBox);
		}

		int ProviderCount =m_pBuff->GetHeadI();
		for(int i=0;i<ProviderCount;i++)
		{
			__Provider* pProvider=new __Provider;
			pProvider->ReadFromBuffer(m_pBuff);
			m_ProviderList.append(pProvider);
		}

		int CarrierCount =m_pBuff->GetHeadI();
		for(int i=0;i<CarrierCount ;i++)
		{
			__Carrier* pCarrier=new __Carrier;
			pCarrier->ReadFromBuffer(m_pBuff);
			m_CarrierList.append(pCarrier);
		}

		int PalleteCount =m_pBuff->GetHeadI();
		for(int i=0;i<PalleteCount;i++)
		{
			__Pallete* pPallete=new __Pallete;
			pPallete->ReadFromBuffer(m_pBuff);
			m_PalleteList.append(pPallete);
		}

		file.close();

	}
		
}
#pragma endregion

#pragma region "CProject::~CProject(void)"
CProject::~CProject(void)
{


	QString fileName2 = QCoreApplication::applicationDirPath();
	fileName2 += "/DATA.dat";

	if(m_pBuff)
	{
		m_pBuff->ClearMem();
		m_pBuff=NULL;

	}

	
	m_pBuff=new CBuffer();


	m_pBuff=new CBuffer();
	QFile file(fileName2 );
	if(file.open(QIODevice::ReadWrite))
	{	

		int CargoCount =m_CargoList.count();
		m_pBuff->AddTail(CargoCount);
		for(int i=0;i<m_CargoList.count();i++)
		{
			__BoxRow* pBox=m_CargoList.at(i);
			pBox->SaveToBuffer(m_pBuff);
		}

		int ProviderCount =m_ProviderList.count();
		m_pBuff->AddTail(ProviderCount);
		for(int i=0;i<m_ProviderList.count();i++)
		{
			__Provider* pProvider=m_ProviderList.at(i);
			pProvider->SaveToBuffer(m_pBuff);
		}

		int CarrierCount =m_CarrierList.count();
		m_pBuff->AddTail(CarrierCount);
		for(int i=0;i<m_CarrierList.count();i++)
		{
			__Carrier* pCarrier=m_CarrierList.at(i);
			pCarrier->SaveToBuffer(m_pBuff);
		}

		int PalleteCount =m_PalleteList.count();
		m_pBuff->AddTail(PalleteCount);
		for(int i=0;i<m_PalleteList.count();i++)
		{
			__Pallete* pPallete=m_PalleteList.at(i);
			pPallete->SaveToBuffer(m_pBuff);
		}

		file.write((const char*)m_pBuff->m_Data,m_pBuff->m_size);
		file.close();


	}

	ClearAll();

}
#pragma endregion

#pragma region "void CProject::AddNewCargo()"
void CProject::AddNewCargo()
{
	__BoxRow* cargo=new __BoxRow;
	m_CargoList.append(cargo);

}
#pragma endregion

#pragma region "void CProject::AddNewProvider()"
void CProject::AddNewProvider()
{
	__Provider* prov=new __Provider;
	m_ProviderList.append(prov);
}
#pragma endregion

#pragma region "void CProject::AddNewCarrier()"
void CProject::AddNewCarrier()
{
	__Carrier* car=new __Carrier;
	m_CarrierList.append(car);
}
#pragma endregion

#pragma region "void CProject::AddNewPallete()"
void CProject::AddNewPallete()
{
	__Pallete* pal=new __Pallete;
	m_PalleteList.append(pal);
}
#pragma endregion 

#pragma region "void CProject::AddNewOrder()"
void CProject::AddNewOrder()
{
	__Order* ord=new __Order;
	m_OrderList.append(ord);
}
#pragma endregion 

#pragma region "void CProject::ClearAll()"
void CProject::ClearAll()
{
	delete m_pBuff;
	m_pBuff=NULL;
	foreach(__BoxRow* item,m_CargoList)
	{
		delete item;
		item=NULL;
	}
	m_CargoList.clear();
	foreach(__Provider* item,m_ProviderList)
	{
		delete item;
		item=NULL;
	}
	m_ProviderList.clear();
	foreach(__Carrier* item,m_CarrierList)
	{
		delete item;
		item=NULL;
	}
	m_CarrierList.clear();
	foreach(__Pallete* item,m_PalleteList)
	{
		delete item;
		item=NULL;
	}
	m_PalleteList.clear();
	foreach(__Order* item,m_OrderList)
	{
		delete item;
		item=NULL;
	}
	m_OrderList.clear();
}
#pragma endregion 

#pragma region "void CProject::newProject()"
void CProject::newProject()
{
	ClearAll();
}
#pragma endregion 

#pragma region "void CProject::closeProject()"
void CProject::closeProject()
{

}
#pragma endregion 

#pragma region "bool CProject::Open(QString fileName)"
bool CProject::Open(QString fileName)
{
	m_filePath = fileName;

	delete m_pBuff;
	m_pBuff=NULL;

	foreach(__Order* item,m_OrderList)
	{
		delete item;
		item=NULL;
	}
	m_OrderList.clear();

	QFile file1(fileName);
	if(!file1.open(QIODevice::ReadOnly))
		return false;

	QByteArray bArray1;
	bArray1=file1.readAll();
	char* ch1=bArray1.data();
	if(!m_pBuff)
		m_pBuff=new CBuffer();
	m_pBuff->SetBuffer(ch1,bArray1.length());

	int OrderCount =m_pBuff->GetHeadI();
	for(int i=0;i<OrderCount;i++)
	{
		__Order* pOrder=new __Order;
		pOrder->ReadFromBuffer(m_pBuff);
		m_OrderList.append(pOrder);
	}

	file1.close();	

	return true;
}
#pragma endregion 

#pragma region "void CProject::Save()"
void CProject::Save()
{


}
#pragma endregion 

#pragma region "void CProject::SaveAs(QString fileName)"
void CProject::SaveAs(QString fileName)
{
	m_pBuff->ClearMem();
	delete m_pBuff;
	m_pBuff=NULL;
	m_pBuff=new CBuffer();
	QFile file1(fileName);
	if(!file1.open(QIODevice::ReadWrite))
		return ;

	int OrderCount =m_OrderList.count();
	m_pBuff->AddTail(OrderCount);
	for(int i=0;i<m_OrderList.count();i++)
	{
		__Order* pOrder=m_OrderList.at(i);
		pOrder->SaveToBuffer(m_pBuff);
	}

	file1.write((const char*)m_pBuff->m_Data,m_pBuff->m_size);
	file1.close();
	return ;

}
#pragma endregion 

#pragma region "__BoxRow* CProject::GetBox(int id)" 
__BoxRow* CProject::GetBox(int id)
{
	for(int i=0;i<m_CargoList.count();i++)
	{
		__BoxRow* box=m_CargoList.at(i);
		if(box->id==id)
			return box;
	}
	return NULL;
}
#pragma endregion

#pragma region "__BoxRow* CProject::GetBoxByName			(QString name)"
__BoxRow* CProject::GetBoxByName			(QString name)
{
	for(int i=0;i<m_CargoList.count();i++)
	{
		__BoxRow* box=m_CargoList.at(i);
		if(box->Desc==name)
			return box;
	}
	return NULL;

}
#pragma endregion

#pragma region "__Provider* CProject::GetProvider(int id)"
__Provider* CProject::GetProvider(int id)
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

#pragma region "__Provider* CProject::GetProviderByName(QString name)"
__Provider* CProject::GetProviderByName(QString name)
{
		for(int i=0;i<m_ProviderList.count();i++)
	{
		__Provider* pro=m_ProviderList.at(i);
		if(pro->name==name)
			return pro;
	}
	return NULL;
}
#pragma endregion

#pragma region  "void CProject::Compute()"
void CProject::Compute()
{
	
	if(m_CarrierList.count()<=0)
		return;

	while(m_AllPathes.count()!=0)
	{
		Path* temp  =  m_AllPathes.takeFirst();
		if(temp->m_Carrier!= NULL)
		{
			delete temp->m_Carrier;
			temp->m_Carrier = NULL;
		}
		while(temp->m_Node.count()!=0)
		{
			Node *nTemp = temp->m_Node.takeFirst(); 
			if(nTemp!=NULL)
			{
				delete nTemp;
				nTemp = NULL;
			}
		}
	}

	//__Carrier* firstCarrier=(__Carrier*)m_CarrierList.at(0);
	int totalCount=0;
	foreach(__Order* order,m_OrderList)
	{
		totalCount+=order->count;
	}

	//if(firstCarrier->m_FloorAreas.count() == 0 && firstCarrier->m_TopAreas.count() == 0)
	//{
	//	CArea* pTotalFloor = new CArea(firstCarrier->Width,firstCarrier->Length,0,0,0,firstCarrier->Height , firstCarrier->MaxLoad);
	//	firstCarrier->m_FloorAreas.append(pTotalFloor);
	//}

    m_pCPathOptimizer = new CPathOptimizer(m_CarrierList, m_OrderList , m_CargoList , m_ProviderList, 0 , 0 , m_fMode);

    QString mString;
    m_pCPathOptimizer->Compute(mString);


   /* m_pCPathOptimizer->Optimization();
    m_pCPathOptimizer->fillAllPath();

    m_pCPathOptimizer->OptimizeCarrier();


    if(!m_fMode)
    m_pCPathOptimizer->ReArange();*/

    m_AllPathes = m_pCPathOptimizer->m_AllPathes;
    
	return ;
}
#pragma endregion 

#pragma region "void CProject::ComputeByTSPack()"
void CProject::ComputeByTSPack()
{


	//double** AllArray = new double*[100];
	//for (int i= 0 ; i < 100 ; i++)
	//{
	//	AllArray[i] = new double[3,3];
	//	AllArray[i][1,1] = 10;
	//}


	//if(m_CarrierList.count()<=0)
	//	return;
	//__Carrier* firstCarrier=(__Carrier*)m_CarrierList.at(0);
	//int totalCount=0;
	//foreach(__Order* order,m_OrderList)
	//{
	//	totalCount+=order->count;
	//}

	//int n=totalCount;
	//int W=firstCarrier->Width;
	//int H=firstCarrier->Length;
	//int D=firstCarrier->Height;
	//int d=3;

	//int* w=new int[d*n];
	//int** ww;//=new int[d][n];
	//int* WP=new int[d*1];

	//ww = (int**)calloc(d, sizeof(int*));
	//int** XX;
	//XX = (int**)calloc(d, sizeof(int*));
	//for (int i = 0; i < d; i++) {
	//	ww[i] = (int*)calloc(n, sizeof(int));
	//	XX[i] = (int*)calloc(n, sizeof(int));
	//}


	//WP[0]=D;
	//WP[1]=W;
	//WP[2]=H;
	//int lb=1;
	//int TL=1000;
	//int ub0=0;
	//int* X=new int[d*n];
	////int** X=new int[d][n];
	////int** XX=&w;
	//int* b=new int[n*1];
	//int i=0;

	//
	//foreach(__Order* order,m_OrderList)
	//{
	//	for (int j = 0;j<order->count;j++)
	//	{
	//		order->PrepareBoxList();
	//		__BoxRow* box=GetBox(order->BoxID);
	//		if(!box)
	//			return;
	//		ww[0][i]=box->PaleteSizeW;
	//		ww[1][i]=box->PaleteSizeL;
	//		ww[2][i]=box->PaleteSizeH;
	//		XX[0][i]=0;
	//		XX[1][i]=0;
	//		XX[2][i]=0;
	//		totalCount+=order->count;
	//		i++;
	//	}

	//}
	//TSPackClass cl;
	//lb = cl.lower(d, n, ww, WP);
	//cl.TSpack(d,n,ww,WP,lb,TL,&ub0,XX,b);
	//int correct = cl.checkfs(d, n, ww, WP, XX, b);

	//i=0;
	//foreach(__Order* order,m_OrderList)
	//{
	//	for(int j=0;j<order->count;j++)
	//	{
	//		order->m_BoxList[j].x=XX[0][i]+(b[i])*(W+20);
	//		order->m_BoxList[j].y=XX[1][i];
	//		order->m_BoxList[j].z=XX[2][i];

	//		i++;
	//	}
	//}

}
#pragma endregion

