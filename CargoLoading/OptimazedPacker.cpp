#include "OptimazedPacker.h"

#pragma region"COptimazedPacker::COptimazedPacker(__Carrier* Carrier, QList<Node*> NodeList  , QList<__BoxRow*> CargoList)"
COptimazedPacker::COptimazedPacker(__Carrier* Carrier, QList<Node*> NodeList  , QList<__BoxRow*> CargoList)
{
	m_Carrier = Carrier;

	m_CargoList = CargoList;

	for (int i = 0 ;i<NodeList.count();i++)
	{
		Node* node = NodeList.at(i);
		m_OrderList.append(node->m_Order);
		for (int j = 0;j<node->m_Order->count;j++)
		{
			__BoxRow* box=GetBox(node->m_Order->BoxID);
			box->NodeId = node->id;
			addBoxRow(box);
		}
	}

	m_FloorAreas = Carrier->m_FloorAreas;
	m_TopAreas = Carrier->m_TopAreas ;


	for (int i = 0; i < m_ListBoxPairs.count(); i++)
		m_UnAssignedBoxes.append(m_ListBoxPairs.at(i)->pBox);
}
#pragma endregion

#pragma region"COptimazedPacker::~COptimazedPacker(void)"
COptimazedPacker::~COptimazedPacker(void)
{
	ClearAll();	

	//delete m_pClosest;
}
#pragma endregion

#pragma region"void COptimazedPacker::Compute(QString & OutputMessage )"
bool COptimazedPacker::Compute(QString & OutputMessage)
{
	if(m_UnAssignedBoxes.count() == 0)
	{
		updateAllAreas();
		OutputMessage = "Computation Successfully finished";
		ClearMatrixs();
		return true;
	}

	if(m_TopAreas.count() == 0)
	{	
		if(m_FloorAreas.count()==0)
		{
			updateAllAreas();
			OutputMessage = "It is impossible to put the damn packets into this container";
			ClearMatrixs();
			return false;
		}

		if(!ChooseClosestAreaToCenter(false))
		{
			ClearMatrixs();
			return Compute(OutputMessage);
		}

		ClearMatrixs();

		buildMatrix();

		if(buildNominate())
		{
			putXYZinOrderList(mNominate2.at(0),false);
			ClearMatrixs();
			return Compute(OutputMessage);
		}

		double mMaxSum = 0;

		if(mNominate2.count() == 0)
		{
			ClearMatrixs();

			buildMatrixAnyway();

			if(buildNominate())
			{
				putXYZinOrderList(mNominate2.at(0),false);
				ClearMatrixs();

				return Compute(OutputMessage);
			}

			if(mNominate2.count() == 0)
			{
				for (int i = 0 ; i < m_FloorAreas.count() ; i++)
				{
					CArea* Temp = m_FloorAreas.at(i);
					if(Temp->isEqual(m_pClosest))
					{

						if(!isInList(false))
						{
							m_FloorAreas.removeAt(i);
							m_ALLFloorAreas.append(Temp);
						}

						ClearMatrixs();

						return Compute(OutputMessage);
					}
				}
			}
		}


		for(int i = 0; i < mNominate2.count(); i++)
		{
			MATRIX* matrix = mNominate2.at(i);

			double max = 0;

			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if(matrix->cMatrix[i][j]!=-1)
						max += matrix->cMatrix[i][j];
				}
			}

			if ( max > mMaxSum)
				mMaxSum = max;
		}



		for(int i = 0; i < mNominate2.count(); i++)
		{
			MATRIX* matrix = mNominate2.at(i);

			double max = 0;

			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if(matrix->cMatrix[i][j]!=-1)
						max += matrix->cMatrix[i][j];
				}
			}

			if ( max == mMaxSum)
			{
				putXYZinOrderList(matrix,false);
				ClearMatrixs();

				return Compute(OutputMessage);
			}
		}
	}

	else
	{
		if(!ChooseClosestAreaToCenter(true))
		{
			ClearMatrixs();
			return Compute(OutputMessage);
		}

		ClearMatrixs();

		buildMatrix();

		if(buildNominate())
		{
			putXYZinOrderList(mNominate2.at(0),true);
			ClearMatrixs();

			return Compute(OutputMessage);
		}

		double mMaxSum = 0;

		if(mNominate2.count() == 0)
		{
			ClearMatrixs();

			buildMatrixAnyway();

			if(buildNominate())
			{
				putXYZinOrderList(mNominate2.at(0),true);
				ClearMatrixs();

				return Compute(OutputMessage);
			}

			if(mNominate2.count() == 0)
			{
				for (int i = 0 ; i < m_TopAreas.count() ; i++)
				{
					CArea* Temp = m_TopAreas.at(i);
					if(Temp->isEqual(m_pClosest))
					{

						if(!isInList(true))
						{
							m_TopAreas.removeAt(i);
							m_ALLTopAreas.append(Temp);
						}

						ClearMatrixs();

						return Compute(OutputMessage);
					}
				}
			}
		}


		for(int i = 0; i < mNominate2.count(); i++)
		{
			MATRIX* matrix = mNominate2.at(i);

			double max = 0;

			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if(matrix->cMatrix[i][j]!=-1)
						max += matrix->cMatrix[i][j];
				}
			}

			if ( max > mMaxSum	&& matrix->isValid())
				mMaxSum = max;
		}



		for(int i = 0; i < mNominate2.count(); i++)
		{
			MATRIX* matrix = mNominate2.at(i);

			double max = 0;

			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if(matrix->cMatrix[i][j]!=-1)
						max += matrix->cMatrix[i][j];
				}
			}

			if ( max == mMaxSum)
			{
				putXYZinOrderList(matrix,true);
				ClearMatrixs();
				return Compute(OutputMessage);
			}
		}

	}



	return false;
}
#pragma endregion

#pragma region"void COptimazedPacker::putXYZinOrderList(MATRIX* matrix , bool isTopArea)"
void COptimazedPacker::putXYZinOrderList(MATRIX* matrix , bool isTopArea)
{

	m_pClosest->m_MaxLoad -= matrix->pBoxRow->PaleteWeight;

	for(int q = 0;q<m_OrderList.count();q++)
	{
		__Order* order = m_OrderList.at(q);
		if (order->BoxID == matrix->pBoxRow->id)
		{
			__BoxInContainer* mBox =new __BoxInContainer();

			mBox->x = (int) m_pClosest->m_X;
			mBox->y = (int) m_pClosest->m_Y;
			mBox->z = (int) m_pClosest->m_FloorZ;

			mBox->NodeId = matrix->pBoxRow->NodeId;

			int w = 0, h = 0, d = 0;
			int i[2] ,j[2];
			int t = 0;

			for (int ii = 0 ; ii < 2 ; ii++)
			{
				for (int jj = 0 ; jj < 3 ; jj++)
				{
					if (matrix->cMatrix[ii][jj] != -1)
					{
						i[t] = ii;
						j[t] = jj;
						t++;
					}
				}
			}

			t = 0;

			bool isWW = false , isWL = false , isWH = false , isHW = false , isHH = false , isHL = false ;

			if ( i[0] == 0 )
			{
				if (j[0] == 0)
				{
					w = matrix->pBoxRow->PaleteSizeW;
					isWW = true;
				}
				if (j[0] == 1)
				{
					w = matrix->pBoxRow->PaleteSizeL;
					isWL = true;
				}
				if (j[0] == 2)
				{
					w = matrix->pBoxRow->PaleteSizeH;
					isWH = true;
				}
			}
			if ( i[0] == 1 )
			{
				if (j[0] == 0)
				{
					h = matrix->pBoxRow->PaleteSizeW;
					isHW = true;
				}
				if (j[0] == 1)
				{
					h = matrix->pBoxRow->PaleteSizeL;
					isHL = true;
				}
				if (j[0] == 2)
				{
					h = matrix->pBoxRow->PaleteSizeH;
					isHH = true;
				}
			}



			if ( i[1] == 0 )
			{
				if (j[1] == 0)
				{
					w = matrix->pBoxRow->PaleteSizeW;
					isWW = true;
				}
				if (j[1] == 1)
				{
					w = matrix->pBoxRow->PaleteSizeL;
					isWL = true;
				}
				if (j[1] == 2)
				{
					w = matrix->pBoxRow->PaleteSizeH;
					isWH = true;
				}
			}
			if ( i[1] == 1 )
			{
				if (j[1] == 0)
				{
					h = matrix->pBoxRow->PaleteSizeW;
					isHW = true;
				}
				if (j[1] == 1)
				{
					h = matrix->pBoxRow->PaleteSizeL;
					isHL = true;
				}
				if (j[1] == 2)
				{
					h = matrix->pBoxRow->PaleteSizeH;
					isHH = true;
				}
			}


			if(isWW)
			{
				if(isHH)
					d = matrix->pBoxRow->PaleteSizeL;
				if(isHL)
					d = matrix->pBoxRow->PaleteSizeH;
			}

			if(isWH)
			{
				if(isHW)
					d = matrix->pBoxRow->PaleteSizeL;
				if(isHL)
					d = matrix->pBoxRow->PaleteSizeW;
			}

			if(isWL)
			{
				if(isHW)
					d = matrix->pBoxRow->PaleteSizeH;
				if(isHH)
					d = matrix->pBoxRow->PaleteSizeW;
			}

			mBox->w = w;
			mBox->h = h;
			mBox->d = d;


			if(w==0||h==0||d==0)
			{
				if(mBox!=NULL)
				{
					delete mBox;
					mBox = NULL;
				}
				return;
			}

			matrix->pBoxRow->PaleteSizeW = w;
			matrix->pBoxRow->PaleteSizeL = h;
			matrix->pBoxRow->PaleteSizeH = d;

			mBox->ContainerID = 0;


			m_Carrier->m_BoxList.append(mBox);
			break;
		}
	}


	if(isTopArea)
	{
		CArea* temp;
		if( m_pClosest->m_MaxLoad > matrix->pBoxRow->MaxLoad)
			temp  = new CArea(matrix->pBoxRow->PaleteSizeW ,matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X , m_pClosest->m_Y , m_pClosest->m_FloorZ + matrix->pBoxRow->PaleteSizeH , m_pClosest->m_TopZ , matrix->pBoxRow->MaxLoad - matrix->pBoxRow->PaleteWeight);
		else
			temp  = new CArea(matrix->pBoxRow->PaleteSizeW ,matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X , m_pClosest->m_Y , m_pClosest->m_FloorZ + matrix->pBoxRow->PaleteSizeH , m_pClosest->m_TopZ , m_pClosest->m_MaxLoad  - matrix->pBoxRow->PaleteWeight);

		m_TopAreas.append(temp);

		seperateTopArea( matrix);

	}
	else
	{

		CArea* temp;
		if( m_pClosest->m_MaxLoad > matrix->pBoxRow->MaxLoad)
			temp  = new CArea(matrix->pBoxRow->PaleteSizeW ,matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X , m_pClosest->m_Y , matrix->pBoxRow->PaleteSizeH , m_pClosest->m_TopZ , matrix->pBoxRow->MaxLoad - matrix->pBoxRow->PaleteWeight);
		else
			temp  = new CArea(matrix->pBoxRow->PaleteSizeW ,matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X , m_pClosest->m_Y , matrix->pBoxRow->PaleteSizeH , m_pClosest->m_TopZ , m_pClosest->m_MaxLoad  - matrix->pBoxRow->PaleteWeight);

		m_TopAreas.append(temp);

		seperateFloorArea( matrix);

	}

	for(int i = 0; i < m_UnAssignedBoxes.count() ; i++)
	{
		__BoxRow* TEMP = m_UnAssignedBoxes.at(i);
		if(TEMP->id == matrix->pBoxRow->id)
		{
			m_UnAssignedBoxes.removeAt(i);
			return;
		}
	}

}
#pragma endregion

#pragma region"void COptimazedPacker::seperateFloorArea(MATRIX* matrix)"
void COptimazedPacker::seperateFloorArea(MATRIX* matrix)
{
	double SArea = m_pClosest->m_H * m_pClosest->m_W;

	CArea* temp1 = new CArea( m_pClosest->m_W - matrix->pBoxRow->PaleteSizeW, m_pClosest->m_H, m_pClosest->m_X+matrix->pBoxRow->PaleteSizeW, m_pClosest->m_Y, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);
	CArea* temp2 = new CArea(matrix->pBoxRow->PaleteSizeW, m_pClosest->m_H - matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X, m_pClosest->m_Y + matrix->pBoxRow->PaleteSizeL, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);


	CArea* temp3 = new CArea( m_pClosest->m_W - matrix->pBoxRow->PaleteSizeW,matrix->pBoxRow->PaleteSizeL, m_pClosest->m_X+matrix->pBoxRow->PaleteSizeW, m_pClosest->m_Y, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);
	CArea* temp4 = new CArea( m_pClosest->m_W , m_pClosest->m_H - matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X, m_pClosest->m_Y + matrix->pBoxRow->PaleteSizeL, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);

	double S[4];

	S[0] = temp1->m_W * temp1->m_H;
	S[1] = temp2->m_W * temp2->m_H;
	S[2] = temp3->m_W * temp3->m_H;
	S[3] = temp4->m_W * temp4->m_H;


	temp1->m_MaxLoad = (S[0]/SArea) * temp1->m_MaxLoad;
	temp2->m_MaxLoad = (S[1]/SArea) * temp2->m_MaxLoad;
	temp3->m_MaxLoad = (S[2]/SArea) * temp3->m_MaxLoad;
	temp4->m_MaxLoad = (S[3]/SArea) * temp4->m_MaxLoad;


	double max = 0;
	for (int i = 0 ; i< 4 ; i++)
	{
		if(max<S[i])
			max = S[i];
	}

	int num = 0;
	for (int i = 0 ; i< 4 ; i++)
	{
		if(max==S[i])
			num = i;
	}

	switch (num)
	{
	case 0:
	case 1:
		if(temp1->m_W > 0 && temp1->m_H > 0 )
			//if(!MergeArea(temp1, false))
				m_FloorAreas.append(temp1);
		else
		{
			if(temp1 != NULL)
			{
				delete temp1;
				temp1 = NULL;
			}
		}
		if(temp2->m_W > 0 && temp2->m_H > 0 )
			//if(!MergeArea(temp2, false))
				m_FloorAreas.append(temp2);
		else
		{
			if(temp2 != NULL)
			{
				delete temp2;
				temp2 = NULL;
			}
		}
		if(temp3 != NULL)
		{
			delete temp3;
			temp3= NULL;
		}
		if(temp4 != NULL)
		{
			delete temp4;
			temp4 = NULL;
		}
		break;
	case 2:
	case 3:
		if(temp3->m_W > 0 && temp3->m_H > 0 )
		{
			//if(!MergeArea(temp3 , false))
				m_FloorAreas.append(temp3);
		}
		else
		{
			if(temp3 != NULL)
			{
				delete temp3;
				temp3 = NULL;
			}
		}
		if(temp4->m_W > 0 && temp4->m_H > 0 )
			//if(!MergeArea(temp4, false))
				m_FloorAreas.append(temp4);
		else
		{
			if(temp4 != NULL)
			{
				delete temp4;
				temp4 = NULL;
			}
		}
		if(temp1 != NULL)
		{
			delete temp1;
			temp1 = NULL;
		}
		if(temp2 != NULL)
		{
			delete temp2;
			temp2 = NULL;
		}
		break;
	}

	for(int i = 0; i < m_FloorAreas.count() ; i++)
	{
		CArea* TEMP = m_FloorAreas.at(i);
		if(TEMP == m_pClosest)
		{
			m_FloorAreas.removeAt(i);
			for (int j = 0 ; j < m_ALLFloorAreas.count() ; j++)
			{
				CArea* Temp = m_ALLFloorAreas.at(j);
				if(Temp->isEqual(m_pClosest))
				{
					m_ALLFloorAreas.removeAt(j);
				}
			}
			if(m_pClosest!= NULL)
			{
				//delete m_pClosest;
				//m_pClosest = NULL;
			}
			return;
		}
	}

}
#pragma endregion

#pragma region"void COptimazedPacker::seperateTopArea(MATRIX* matrix)"
void COptimazedPacker::seperateTopArea(MATRIX* matrix)
{
	double SArea = m_pClosest->m_H * m_pClosest->m_W;

	CArea* temp1 = new CArea( m_pClosest->m_W - matrix->pBoxRow->PaleteSizeW, m_pClosest->m_H, m_pClosest->m_X+matrix->pBoxRow->PaleteSizeW, m_pClosest->m_Y, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);
	CArea* temp2 = new CArea(matrix->pBoxRow->PaleteSizeW, m_pClosest->m_H - matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X, m_pClosest->m_Y + matrix->pBoxRow->PaleteSizeL, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);

	CArea* temp3 = new CArea( m_pClosest->m_W - matrix->pBoxRow->PaleteSizeW,matrix->pBoxRow->PaleteSizeL, m_pClosest->m_X+matrix->pBoxRow->PaleteSizeW, m_pClosest->m_Y, m_pClosest->m_FloorZ, m_pClosest->m_TopZ, m_pClosest->m_MaxLoad);
	CArea* temp4 = new CArea( m_pClosest->m_W , m_pClosest->m_H - matrix->pBoxRow->PaleteSizeL , m_pClosest->m_X, m_pClosest->m_Y + matrix->pBoxRow->PaleteSizeL, m_pClosest->m_FloorZ, m_pClosest->m_TopZ,m_pClosest->m_MaxLoad);

	double S[4];

	S[0] = temp1->m_W * temp1->m_H;
	S[1] = temp2->m_W * temp2->m_H;
	S[2] = temp3->m_W * temp3->m_H;
	S[3] = temp4->m_W * temp4->m_H;

	temp1->m_MaxLoad = (S[0]/SArea) * temp1->m_MaxLoad;
	temp2->m_MaxLoad = (S[1]/SArea) * temp2->m_MaxLoad;
	temp3->m_MaxLoad = (S[2]/SArea) * temp3->m_MaxLoad;
	temp4->m_MaxLoad = (S[3]/SArea) * temp4->m_MaxLoad;

	double max = 0;
	for (int i = 0 ; i< 4 ; i++)
	{
		if(max<S[i])
			max = S[i];
	}

	int num = 0;
	for (int i = 0 ; i< 4 ; i++)
	{
		if(max==S[i])
			num = i;
	}

	switch (num)
	{
	case 0:
	case 1:
		if(temp1->m_W > 0 && temp1->m_H > 0 )
			//if(!MergeArea(temp1, true))
				m_TopAreas.append(temp1);
		else
		{
			if(temp1 != NULL)
			{
				delete temp1;
				temp1 = NULL;
			}
		}
		if(temp2->m_W > 0 && temp2->m_H > 0 )
			//if(!MergeArea(temp2, true))
				m_TopAreas.append(temp2);
		else
		{
			if(temp2 != NULL)
			{
				delete temp2;
				temp2 = NULL;
			}
		}
		if(temp3 != NULL)
		{
			delete temp3;
			temp3= NULL;
		}
		if(temp4 != NULL)
		{
			delete temp4;
			temp4 = NULL;
		}
		break;
	case 2:
	case 3:
		if(temp3->m_W > 0 && temp3->m_H > 0 )
			//if(!MergeArea(temp3, true))
				m_TopAreas.append(temp3);
		else
		{
			if(temp3 != NULL)
			{
				delete temp3;
				temp3 = NULL;
			}
		}
		if(temp4->m_W > 0 && temp4->m_H > 0 )
			//if(!MergeArea(temp4, true))
				m_TopAreas.append(temp4);
		else
		{
			if(temp4 != NULL)
			{
				delete temp4;
				temp4 = NULL;
			}
		}
		if(temp1 != NULL)
		{
			delete temp1;
			temp1 = NULL;
		}
		if(temp2 != NULL)
		{
			delete temp2;
			temp2 = NULL;
		}
		break;
	}

	for(int i = 0; i < m_TopAreas.count() ; i++)
	{
		CArea* TEMP = m_TopAreas.at(i);
		if(TEMP == m_pClosest)
		{
			m_TopAreas.removeAt(i);
			for (int j = 0 ; j < m_ALLTopAreas.count() ; j++)
			{
				CArea* Temp = m_ALLTopAreas.at(j);
				if(Temp->isEqual(m_pClosest))
				{
					m_ALLTopAreas.removeAt(j);
				}
			}
			if(m_pClosest!= NULL)
			{
				//delete m_pClosest;
				//m_pClosest = NULL;
			}
			return;
		}
	}

}
#pragma endregion

#pragma region"bool COptimazedPacker::isLimitsChackes(MATRIX* mNominate2)"
bool COptimazedPacker::isLimitsChackes(MATRIX* mNominate2)
{
	if( m_pClosest->m_MaxLoad < mNominate2->pBoxRow->PaleteWeight)
		return false;

	QStringList indexList = mNominate2->pBoxRow->FloorIndex.split(" ");

	bool boolList[6];
	for (int ii = 0 ; ii < 6 ; ii++)
	{
		boolList[ii] = false;
	}

	for(int ii = 0 ; ii < indexList.count() ; ii++)
	{
		int tt = indexList.at(ii).toInt();
		boolList[tt-1] = true;
	}

	int i[2] ,j[2];
	int t = 0;

	for (int ii = 0 ; ii < 2 ; ii++)
	{
		for (int jj = 0 ; jj < 3 ; jj++)
		{
			if (mNominate2->cMatrix[ii][jj] != -1)
			{
				i[t] = ii;
				j[t] = jj;
				t++;
			}
		}
	}

	t = 0;

	bool isWW = false , isWL = false , isWH = false , isHW = false , isHH = false , isHL = false ;

	if ( i[0] == 0 )
	{
		if (j[0] == 0)
		{
			isWW = true;
		}
		if (j[0] == 1)
		{
			isWL = true;
		}
		if (j[0] == 2)
		{
			isWH = true;
		}
	}
	if ( i[0] == 1 )
	{
		if (j[0] == 0)
		{
			isHW = true;
		}
		if (j[0] == 1)
		{
			isHL = true;
		}
		if (j[0] == 2)
		{
			isHH = true;
		}
	}



	if ( i[1] == 0 )
	{
		if (j[1] == 0)
		{
			isWW = true;
		}
		if (j[1] == 1)
		{
			isWL = true;
		}
		if (j[1] == 2)
		{
			isWH = true;
		}
	}
	if ( i[1] == 1 )
	{
		if (j[1] == 0)
		{
			isHW = true;
		}
		if (j[1] == 1)
		{
			isHL = true;
		}
		if (j[1] == 2)
		{
			isHH = true;
		}
	}

	if(isWW)
	{
		if(isHH)
			if((boolList[4] || boolList[1]) && ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ >= mNominate2->pBoxRow->PaleteSizeL))
				return true;
		if(isHL)
			if((boolList[5] || boolList[0]) && ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ  >= mNominate2->pBoxRow->PaleteSizeH))
				return true;
	}

	if(isWH)
	{
		if(isHW)
			if((boolList[1] || boolList[4]) && ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ  >= mNominate2->pBoxRow->PaleteSizeL))
				return true;
		if(isHL)
			if((boolList[2] || boolList[3]) && ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ  >= mNominate2->pBoxRow->PaleteSizeW))
				return true;
	}

	if(isWL)
	{
		if(isHW)
			if((boolList[0] || boolList[5]) && ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ  >= mNominate2->pBoxRow->PaleteSizeH))
				return true;
		if(isHH)
			if((boolList[2] || boolList[3]) && ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ  >= mNominate2->pBoxRow->PaleteSizeW))
				return true;
	}

	return false;


}
#pragma endregion

#pragma region"bool COptimazedPacker::isAbleToPut(MATRIX* matrix)"
bool COptimazedPacker::isAbleToPut(MATRIX* matrix)
{
	if( m_pClosest->m_MaxLoad < matrix->pBoxRow->PaleteWeight)
		return false;

	double temp[3][3];

	temp[0][0] = matrix->pBoxRow->PaleteSizeW / m_pClosest->m_W;
	temp[0][1] = matrix->pBoxRow->PaleteSizeW / m_pClosest->m_H;
	temp[0][2] = matrix->pBoxRow->PaleteSizeW / ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ);

	temp[1][0] = matrix->pBoxRow->PaleteSizeL / m_pClosest->m_W;
	temp[1][1] = matrix->pBoxRow->PaleteSizeL / m_pClosest->m_H;
	temp[1][2] = matrix->pBoxRow->PaleteSizeL / ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ);

	temp[2][0] = matrix->pBoxRow->PaleteSizeH / m_pClosest->m_W;
	temp[2][1] = matrix->pBoxRow->PaleteSizeH / m_pClosest->m_H;
	temp[2][2] = matrix->pBoxRow->PaleteSizeH / ( m_pClosest->m_TopZ - m_pClosest->m_FloorZ);

	for(int i = 0 ; i < 3 ; i++)
	{
		if (temp[i][0] > 1 && temp[i][1] > 1 && temp[i][2] > 1)
		{
			return false;
		}
	}
	return true;

}
#pragma endregion

#pragma region"bool COptimazedPacker::ChooseClosestAreaToCenter(bool isTopArea)"
bool COptimazedPacker::ChooseClosestAreaToCenter(bool isTopArea)
{
	double distToCenter = 1.79769e+308;
	double MinDistToCenter = 1.79769e+308;

	m_pClosest = NULL;

	if(isTopArea)
	{
		for (int i = 0 ; i< m_TopAreas.count() ; i++)
		{
			CArea* PArea = m_TopAreas.at(i);
			distToCenter = pow(PArea->m_X,2)+pow(PArea->m_Y,2)+pow(PArea->m_FloorZ,2);
			distToCenter = pow(distToCenter,0.5);

			if(distToCenter < MinDistToCenter)
			{
				MinDistToCenter = distToCenter;
				m_pClosest = PArea;
			}
		}
	}
	else
	{
		for (int i = 0 ; i< m_FloorAreas.count() ; i++)
		{
			CArea* PArea = m_FloorAreas.at(i);
			distToCenter = pow(PArea->m_X,2)+pow(PArea->m_Y,2)+pow(PArea->m_FloorZ,2);
			distToCenter = pow(distToCenter,0.5);

			if(distToCenter < MinDistToCenter)
			{
				MinDistToCenter = distToCenter;
				m_pClosest = PArea;
			}
		}

	}

	if(m_pClosest==NULL)
		return false;

	return true;
}
#pragma endregion

#pragma region"void COptimazedPacker::ClearAll()"
void COptimazedPacker::ClearAll()
{
	ClearMatrixs();

	while (m_FloorAreas.count()!=0)
	{
		CArea* temp = m_FloorAreas.takeFirst();
		if(temp!=NULL)
		{
			delete temp;
			temp = NULL;
		}
	}
	while (m_TopAreas.count()!=0)
	{
		CArea* temp = m_TopAreas.takeFirst();
		if(temp!=NULL)
		{
			delete temp;
			temp = NULL;
		}
	}

	while (m_ListBoxPairs.count()!=0)
	{
		BOX_PAIR* temp = m_ListBoxPairs.takeFirst();
		if(temp!=NULL)
		{
			delete temp;
			temp = NULL;
		}
	}


}
#pragma endregion

#pragma region"void COptimazedPacker::ClearMatrixs()"
void COptimazedPacker::ClearMatrixs()
{

	while (!mNominate.isEmpty())
	{
		MATRIX* TEMP = mNominate.takeFirst();
		for (int i = 0 ; i < mNominate2.count() ; i++)
		{
			MATRIX* TEMP2 = mNominate2.at(i);
			if(TEMP2 == TEMP)
				mNominate2.removeAt(i);
		}
		if(TEMP!=NULL)
		{
			delete TEMP;
			TEMP = NULL;
		}
	}


	while (!mNominate2.isEmpty())
	{
		MATRIX* TEMP = mNominate2.takeFirst();
		for (int i = 0 ; i < mNominate.count() ; i++)
		{
			MATRIX* TEMP2 = mNominate.at(i);
			if(TEMP2 == TEMP)
				mNominate.removeAt(i);
		}
		if(TEMP!=NULL)
		{
			delete TEMP;
			TEMP = NULL;
		}
	}
}
#pragma endregion

#pragma region"void COptimazedPacker::addBoxRow(__BoxRow* BoxRow)"
void COptimazedPacker::addBoxRow(__BoxRow* BoxRow)
{
	BOX_PAIR* pBoxPair = new BOX_PAIR;
	pBoxPair->pBox = new __BoxRow;
	*pBoxPair->pBox = *BoxRow;

	m_ListBoxPairs.append(pBoxPair);
}
#pragma endregion

#pragma region "__BoxRow* COptimazedPacker::GetBox(int id)" 
__BoxRow* COptimazedPacker::GetBox(int id)
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

#pragma region "bool COptimazedPacker::isInList(bool isAllTopArea)" 
bool COptimazedPacker::isInList(bool isAllTopArea)
{
	if(isAllTopArea)
	{
		for(int i = 0 ; i < m_ALLTopAreas.count() ; i++)
		{
			CArea* temp = m_ALLTopAreas.at(i);
			if(temp->isEqual( m_pClosest))
				return true;
		}
	}
	else
	{
		for(int i = 0 ; i < m_ALLFloorAreas.count() ; i++)
		{
			CArea* temp = m_ALLFloorAreas.at(i);
			if(temp->isEqual( m_pClosest))
				return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region"void COptimazedPacker::updateAllAreas()"
void COptimazedPacker::updateAllAreas()
{
	while (!m_TopAreas.isEmpty())
	{
		CArea* Temp = m_TopAreas.takeFirst();
		m_pClosest = Temp;
		if(!isInList(true))
			m_ALLTopAreas.append(Temp);
	}

	while (!m_FloorAreas.isEmpty())
	{
		CArea* Temp = m_FloorAreas.takeFirst();
		m_pClosest = Temp;
		if(!isInList(false))
			m_ALLFloorAreas.append(Temp);
	}

}
#pragma endregion

#pragma region"void COptimazedPacker::buildMatrix()"
void COptimazedPacker::buildMatrix()
{
	double mMax = 0;

	QList<MATRIX*> mMatrix;

	mNominate;

	for(int i = 0 ; i < m_UnAssignedBoxes.count() ; i++)
	{
		__BoxRow* pBoxRow = m_UnAssignedBoxes.at(i);
		MATRIX* pMatrix = new MATRIX();
		pMatrix->cMatrix[0][0] =  pBoxRow->PaleteSizeW / m_pClosest->m_W;
		pMatrix->cMatrix[0][1] =  pBoxRow->PaleteSizeL / m_pClosest->m_W;
		pMatrix->cMatrix[0][2] =  pBoxRow->PaleteSizeH / m_pClosest->m_W;

		pMatrix->cMatrix[1][0] =  pBoxRow->PaleteSizeW / m_pClosest->m_H;
		pMatrix->cMatrix[1][1] =  pBoxRow->PaleteSizeL / m_pClosest->m_H;
		pMatrix->cMatrix[1][2] =  pBoxRow->PaleteSizeH / m_pClosest->m_H;

		pMatrix->pBoxRow  = new __BoxRow();
		*pMatrix->pBoxRow = *pBoxRow;



		if(isAbleToPut( pMatrix))
		{
			pMatrix->CheckLimits();

			if (mMax < pMatrix->FindMax())
				mMax = pMatrix->FindMax();

			mMatrix.append(pMatrix);
		}
		else 
		{
			if(pMatrix!= NULL)
			{
				delete pMatrix;
				pMatrix = NULL;
			}
		}
	}

	for(int tt = 0; tt < mMatrix.count(); tt++)
	{
		MATRIX* matrix = mMatrix.at(tt);
		if(matrix->maxCount(mMax) == 1)
		{
			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if (matrix->cMatrix[i][j] == mMax)
					{
						matrix->maxRow = i;
						matrix->maxCol = j;
					}
				}
			}
			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if((i == matrix->maxRow && j != matrix->maxCol ) || (i != matrix->maxRow && j == matrix->maxCol ))
						matrix->cMatrix[i][j] = -1;
				}
			}
			mNominate.append(matrix);
			mMatrix.removeOne(matrix);
		}
		else if (matrix->maxCount(mMax) > 1)
		{

			int* Row = new int[matrix->maxCount(mMax)];
			int* Col = new int[matrix->maxCount(mMax)];

			int t = 0;

			for (int i = 0; i < 2 ; i++)
			{
				for (int j = 0; j < 3 ; j++)
				{
					if (matrix->cMatrix[i][j] == mMax)
					{
						Row[t] = i;
						Col[t] = j;
						t++;
					}
				}
			}

			t = 0;

			for(int k = 0 ; k < matrix->maxCount(mMax) ; k++)
			{
				MATRIX* TEMP = new MATRIX();
				TEMP->pBoxRow = new __BoxRow();
				*TEMP->pBoxRow = *matrix->pBoxRow;

				TEMP->maxRow = Row[t];
				TEMP->maxCol = Col[t];

				for (int i = 0; i < 2 ; i++)
				{
					for (int j = 0; j < 3 ; j++)
					{
						if((i == Row[t] && j != Col[t]) || (i != Row[t] && j == Col[t]))
							TEMP->cMatrix[i][j] = -1;
						else
							TEMP->cMatrix[i][j] = matrix->cMatrix[i][j];
					}
				}
				mNominate.append(TEMP);
				t++;
			}
			if(matrix!=NULL)
			{
				delete matrix;
				matrix = NULL;
			}
			//mNominate.removeOne(matrix);
			mMatrix.removeAt(tt);
			if(Row!=NULL)
			{
				delete Row;
				Row = NULL;
			}
			if(Col!=NULL)
			{
				delete Col;
				Col = NULL;
			}
		}
	}

	while (!mMatrix.isEmpty())
	{
		MATRIX* TEMP = mMatrix.takeFirst();
		if(TEMP!=NULL)
		{
			delete TEMP;
			TEMP = NULL;
		}
	}

	return;
}
#pragma endregion

#pragma region"void COptimazedPacker::buildMatrixAnyway()"
void COptimazedPacker::buildMatrixAnyway()
{
	QList<MATRIX*> mMatrix;

	for(int i = 0 ; i < m_UnAssignedBoxes.count() ; i++)
	{
		__BoxRow* pBoxRow = m_UnAssignedBoxes.at(i);
		MATRIX* pMatrix = new MATRIX();
		pMatrix->cMatrix[0][0] =  pBoxRow->PaleteSizeW / m_pClosest->m_W;
		pMatrix->cMatrix[0][1] =  pBoxRow->PaleteSizeL / m_pClosest->m_W;
		pMatrix->cMatrix[0][2] =  pBoxRow->PaleteSizeH / m_pClosest->m_W;

		pMatrix->cMatrix[1][0] =  pBoxRow->PaleteSizeW / m_pClosest->m_H;
		pMatrix->cMatrix[1][1] =  pBoxRow->PaleteSizeL / m_pClosest->m_H;
		pMatrix->cMatrix[1][2] =  pBoxRow->PaleteSizeH / m_pClosest->m_H;

		pMatrix->pBoxRow = new __BoxRow();
		*pMatrix->pBoxRow = *pBoxRow;



		if(isAbleToPut( pMatrix))
		{
			pMatrix->CheckLimits();
			if(pMatrix->isValid())
				mMatrix.append(pMatrix);
		}
		else
		{
			if(pMatrix!=NULL)
			{
				delete pMatrix;
				pMatrix = NULL;
			}
		}
	}

	for(int i = 0; i < mMatrix.count(); i++)
	{
		MATRIX* matrix = mMatrix.at(i);


		for(int x = 0 ; x < 2 ;x++)
		{
			for(int y = 0 ; y < 3 ;y++)
			{
				MATRIX* TEMP = new MATRIX();
				TEMP->pBoxRow = new __BoxRow();
				*TEMP->pBoxRow = *matrix->pBoxRow;



				for(int ii = 0 ; ii < 2 ; ii++)
				{
					for(int jj = 0 ; jj < 3 ;jj++)
					{
						if(ii == x && jj == y)
							TEMP->cMatrix[ii][jj] = matrix->cMatrix[ii][jj];
						else if(ii == x || jj == y)
							TEMP->cMatrix[ii][jj] = -1;
						else
							TEMP->cMatrix[ii][jj] = matrix->cMatrix[ii][jj];
					}		
				}
				if(TEMP->isValid())
				{
					mNominate.append(TEMP);
					mMatrix.removeOne(TEMP);
				}
				else
				{
					if(TEMP!=NULL)
					{
						delete TEMP;
						TEMP = NULL;
					}
				}

			}
		}
	}

	while (!mMatrix.isEmpty())
	{
		MATRIX* TEMP = mMatrix.takeFirst();
		if(TEMP!=NULL)
		{
			delete TEMP;
			TEMP = NULL;
		}
	}

	return;
}
#pragma endregion

#pragma region"void COptimazedPacker::buildNominate()"
bool COptimazedPacker::buildNominate()
{
	for(int i = 0; i < mNominate.count(); i++)
	{
		MATRIX* matrix = mNominate.at(i);

		bool firsTime = true;

		if (matrix->validCount()==3)
		{
			int ti[2] ,tj[2];
			int t = 0;

			for (int ii = 0 ; ii < 2 ; ii++)
			{
				for (int jj = 0 ; jj < 3 ; jj++)
				{
					if (matrix->cMatrix[ii][jj] != -1 && !(ii == matrix->maxRow && jj == matrix->maxCol))
					{
						ti[t] = ii;
						tj[t] = jj;
						t++;
					}
				}
			}

			for (int t = 0 ; t<2 ; t++)
			{
				MATRIX* TEMP = new MATRIX();
				TEMP->pBoxRow = new __BoxRow();
				*TEMP->pBoxRow = *matrix->pBoxRow;


				for (int ii = 0 ; ii < 2 ; ii++)
				{
					for (int jj = 0 ; jj < 3 ; jj++)
					{
						if((ii==ti[t] && jj==tj[t]) || (ii == matrix->maxRow && jj == matrix->maxCol))
						{
							TEMP->cMatrix[ii][jj] = matrix->cMatrix[ii][jj];
						}
						else
							TEMP->cMatrix[ii][jj] = -1;
					}
				}

				if(TEMP->isValid() && TEMP->validCount()== 2)
					if(isLimitsChackes(TEMP) )
					{
						if(TEMP->validSum() == 2)
						{
							ClearMatrixs();
							mNominate2.clear();
							mNominate2.append(TEMP);
							return true;
						}
						mNominate2.append(TEMP);
					}
					else
					{
						if(TEMP!=NULL)
						{
							delete TEMP;
							TEMP = NULL;
						}
					}
				else
				{
					if(TEMP!=NULL)
					{
						delete TEMP;
						TEMP = NULL;
					}
				}
			}

		}
		else if (matrix->validCount()==2)
			if(matrix->isValid())
				if(isLimitsChackes(matrix))
				{
					if(matrix->validSum() == 2)
					{
						MATRIX *TEMP = new MATRIX();
						*TEMP = *matrix;
						TEMP->pBoxRow = new __BoxRow();
						*TEMP->pBoxRow = *matrix->pBoxRow;

						ClearMatrixs();
						mNominate2.clear();
						mNominate2.append(TEMP);
						return true;
					}
					mNominate2.append(matrix);
				}

				/*if(matrix->validCount()==2||matrix->validCount()==3)
				{
				mNominate.removeOne(matrix);
				}*/

	}

	while (!mNominate.isEmpty())
	{
		MATRIX* TEMP = mNominate.takeFirst();
		for (int i = 0 ; i < mNominate2.count() ; i++)
		{
			MATRIX* TEMP2 = mNominate2.at(i);
			if(TEMP2 == TEMP)
				mNominate2.removeAt(i);
		}
		if(TEMP!=NULL)
		{
			delete TEMP;
			TEMP = NULL;
		}
	}

	return false;
}
#pragma endregion

#pragma region "bool COptimazedPacker::MergeArea(CArea* area, bool isTopArea)"
bool COptimazedPacker::MergeArea(CArea* area, bool isTopArea)
{
    return false;
	if(!isTopArea)
	{
		for(int i = 0; i < m_FloorAreas.count() ; i++)
		{
			CArea* temp = m_FloorAreas.at(i);
			if(temp->m_FloorZ == area->m_FloorZ)
			{
				if(temp->m_Y == area->m_Y)
				{
					if(temp->m_X == (area->m_X+area->m_W))
					{
						//temp->m_X = area->m_X;

						temp->m_W += area->m_W;
						temp->m_MaxLoad += area->m_MaxLoad;
						return true;
					}
					if((temp->m_X+temp->m_W) == area->m_X)
					{
						temp->m_W += area->m_W;
						temp->m_MaxLoad += area->m_MaxLoad;

						return true;
					}
				}

				if(temp->m_X == area->m_X)
				{
					if(temp->m_Y == (area->m_Y+area->m_H))
					{
						//temp->m_Y = area->m_Y;

						temp->m_H += area->m_H;
						temp->m_MaxLoad += area->m_MaxLoad;
						return true;
					}
					if((temp->m_Y+temp->m_H) == area->m_Y)
					{
						temp->m_H += area->m_H;
						temp->m_MaxLoad += area->m_MaxLoad;

						return true;
					}
				}
			}
		}
	}

	else
	{
		for(int i = 0; i < m_TopAreas.count() ; i++)
		{
			CArea* temp = m_TopAreas.at(i);
			if(temp->m_FloorZ == area->m_FloorZ)
			{
				if(temp->m_Y == area->m_Y)
				{
					if(temp->m_X == (area->m_X+area->m_W))
					{
						//temp->m_X = area->m_X;

						temp->m_W += area->m_W;
						temp->m_MaxLoad += area->m_MaxLoad;
						return true;
					}
					if((temp->m_X+temp->m_W) == area->m_X)
					{
						temp->m_W += area->m_W;
						temp->m_MaxLoad += area->m_MaxLoad;

						return true;
					}
				}

				if(temp->m_X == area->m_X)
				{
					if(temp->m_Y == (area->m_Y+area->m_H))
					{
						//temp->m_Y = area->m_Y;

						temp->m_H += area->m_H;
						temp->m_MaxLoad += area->m_MaxLoad;
						return true;
					}
					if((temp->m_Y+temp->m_H) == area->m_Y)
					{
						temp->m_H += area->m_H;
						temp->m_MaxLoad += area->m_MaxLoad;

						return true;
					}
				}
			}
		}
	}
	return false;
}
#pragma endregion
