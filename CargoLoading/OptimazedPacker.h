#pragma once
#include <qlist.h>
#include <QStringList.h>
#include <math.h>
#include "Project.h"
#ifndef __OPTMAZED_PACKER_H
#define __OPTMAZED_PACKER_H
	

class MATRIX
{
public:
	MATRIX()
	{
		for (int i = 0; i < 2 ; i++)
		{
			for (int j = 0; j < 3 ; j++)
			{
					cMatrix[i][j] = 0;
			}
		}

		maxRow = -1;
		maxCol = -1;
	}
	~MATRIX()
	{
		if(pBoxRow!=NULL)
		{
			delete pBoxRow;
			pBoxRow = NULL;
		}
	}
	double cMatrix[2][3];

	__BoxRow* pBoxRow;

	int maxRow;
	int maxCol;

public:
	void CheckLimits()
	{
		if(cMatrix[0][0] > 1)		cMatrix[0][0] = -1;
		if(cMatrix[0][1] > 1)		cMatrix[0][1] = -1;
		if(cMatrix[0][2] > 1)		cMatrix[0][2] = -1;

		if(cMatrix[1][0] > 1)		cMatrix[1][0] = -1;
		if(cMatrix[1][1] > 1)		cMatrix[1][1] = -1;
		if(cMatrix[1][2] > 1)		cMatrix[1][2] = -1;
	};

	double FindMax()
	{
		double max = 0;

		for (int i = 0; i < 2 ; i++)
		{
			for (int j = 0; j < 3 ; j++)
			{
				if(cMatrix[i][j]>max)
				{
					max = cMatrix[i][j];
				}
			}
		}
		return max;

	};

	bool isIncludeMax(double max)
	{
		for (int i = 0; i < 2 ; i++)
		{
			for (int j = 0; j < 3 ; j++)
			{
				if(cMatrix[i][j] == max)
				{
					return true;
				}
			}
		}
		return false;
	};

	bool isValid()
	{
		if(cMatrix[0][0] == -1 && cMatrix[0][1] == -1 && cMatrix[0][2] == -1)
			return false;
		if(cMatrix[1][0] == -1 && cMatrix[1][1] == -1 && cMatrix[1][2] == -1)
			return false;
		/*if(cMatrix[0][0] == -1 && cMatrix[1][0] == -1 && cMatrix[0][1] == -1 && cMatrix[1][1] == -1)
			return false;
		if(cMatrix[0][0] == -1 && cMatrix[1][0] == -1 && cMatrix[0][2] == -1 && cMatrix[1][2] == -1)
			return false;
		if(cMatrix[0][1] == -1 && cMatrix[1][1] == -1 && cMatrix[0][2] == -1 && cMatrix[1][2] == -1)
			return false;*/

		return true;
	}

	int validCount()
	{
	
		int count = 0;
		for (int i = 0; i < 2 ; i++)
		{
			for (int j = 0; j < 3 ; j++)
			{
				if(cMatrix[i][j] > 0 && cMatrix[i][j] <= 1)
				{
					count++;
				}
			}
		}
		return count;
	};

	int validSum()
	{
		int count = 0;
		for (int i = 0; i < 2 ; i++)
		{
			for (int j = 0; j < 3 ; j++)
			{
				if(cMatrix[i][j] > 0 && cMatrix[i][j] <= 1)
				{
					count += cMatrix[i][j];
				}
			}
		}
		return count;
	};

	int maxCount(double max)
	{
		int count = 0;
		for (int i = 0; i < 2 ; i++)
		{
			for (int j = 0; j < 3 ; j++)
			{
				if(cMatrix[i][j] == max)
				{
					count++;
				}
			}
		}
		return count;
	};
};

class BOX_PAIR
{
public:
	BOX_PAIR()
	{

	}
	~BOX_PAIR()
	{
		if(pBox!= NULL)
		{
			delete pBox;
			pBox = NULL;
		}
	}
	__BoxRow* pBox;

};

class COptimazedPacker
{
public:
	COptimazedPacker(__Carrier* Carrier, QList<Node*> NodeList , QList<__BoxRow*> CargoList);
	~COptimazedPacker(void);

public:
	void addBoxRow(__BoxRow* BoxRow);
	void ClearAll();
	void updateAllAreas();

	bool Compute(QString & OutputMessage);
	bool isLimitsChackes(MATRIX* mNominate2);
	bool isAbleToPut(MATRIX* matrix);

	bool ChooseClosestAreaToCenter(bool isTopArea);

	__BoxRow* GetBox(int id);

	void putXYZinOrderList(MATRIX* BoxRow, bool isTopArea);
	void seperateFloorArea(MATRIX* matrix);
	void seperateTopArea(MATRIX* matrix);
	void ClearMatrixs();

	bool isInList(bool isAllTopArea);

	void buildMatrix();
	void buildMatrixAnyway();

	bool  buildNominate();

	bool MergeArea(CArea* area, bool isTopArea);

public:
	QList<MATRIX*> mNominate;
	QList<MATRIX*> mNominate2;


	QList<CArea*> m_FloorAreas;
	QList<CArea*> m_TopAreas;

	QList<CArea*> m_ALLFloorAreas;
	QList<CArea*> m_ALLTopAreas;

	QList<BOX_PAIR*> m_ListBoxPairs;
	QList<__BoxRow*> m_UnAssignedBoxes;

	QList<__BoxRow*>	m_CargoList;
	QList<__Order*>		m_OrderList;

	__Carrier* m_Carrier;

	CArea* m_pClosest;
};

#endif
