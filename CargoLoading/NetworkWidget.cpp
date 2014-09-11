#pragma region "Includes"
#include "NetworkWidget.h"
#include <SoElapsedTime.h>
#include <SoTrackBallManip.h>
#include "GUIMainWindow.h"


bool CNetworkWidget::c_viewingSt = FALSE;
bool CNetworkWidget::c_wheelSt = FALSE;
bool CNetworkWidget::c_panSt = FALSE;
bool CNetworkWidget::c_selSt = FALSE;
CExaminerViewer *CNetworkWidget::c_pViewer = NULL;

#pragma endregion

#pragma region "Constructor"
CNetworkWidget::CNetworkWidget(CProject *pProject,QWidget *parent,CGUIMainWindow* pMainWindow) : QWidget(parent)
{
	m_pMainWindow=pMainWindow;
	SoQt::init(this);
	m_pProject = pProject;
	

	//createMenu();

	m_pRoot = new SoSeparator;
	m_pRoot->ref();

	m_LastPickedPoint = SbVec3f(0,0,0);
	root= new SoSeparator;
	root->ref();
	pViewer=new SoQtExaminerViewer(this);
	pViewer->setSceneGraph(root);
	root->addChild(new SoDirectionalLight);
	//root->addChild(new SoTrackballManip);
	SoMaterial* myMaterial =new SoMaterial;
	myMaterial->diffuseColor.setValue(0.0,5.0,0.0);
	root->addChild(myMaterial);

	pViewer->setBackgroundColor(SbColor(100,100,100));


	/*root->addChild(AddCubeNode(0,0,0,10,0.1,0.1,1,0,0));
	root->addChild(AddCubeNode(0,0,0,0.1,10,0.1,0,1,0));
	root->addChild(AddCubeNode(0,0,0,0.1,0.1,10,0,0,1));*/

	/*root->addChild(AddCubeNode(0 ,0,0,10,10,1,1,0,0));
	root->addChild(AddCubeNode(11,0,0,10,10,1,0,1,0));*/
	//


	pViewer->setDecoration(FALSE);

	
	

	return;

	//for(int i=0;i<n;i++)

	//{
	//	root->addChild(AddCubeNode(1.1*x[i]/10.0,1.1*y[i]/10.0,1.1*z[i]/10.0,w[i]/10.0,h[i]/10.0,d[i]/10.0,1,0,0));
	//}

	//return;

	//




	//
	//c_pViewer = new CExaminerViewer(this);
	//c_pViewer->setSceneGraph(root);
	////viewer->setBackgroundColor(SbColor(100,100,100));
	//c_pViewer->setComponentCursor(SoQtCursor::BUSY);
	//c_pViewer->setPopupMenuEnabled(FALSE);
	//c_pViewer->setTitle("Details, details");
	//c_pViewer->setGLRenderAction(new SoBoxHighlightRenderAction());

}
#pragma endregion

#pragma region "createMenu()"
void CNetworkWidget::createMenu()
{
	m_CoMenu = new QMenu(0);

	
	m_Update = new QAction("Update",m_CoMenu);
	connect(m_Update , SIGNAL(triggered()),this, SLOT(Update()));




	m_panAction = new QAction("Pan mode",m_CoMenu);
	m_panAction->setCheckable(TRUE);
	connect(m_panAction, SIGNAL(triggered()),this, SLOT(OnPanMode()));

	m_selAction = new QAction("Selection mode",m_CoMenu);
	m_selAction->setCheckable(TRUE);
	connect(m_selAction, SIGNAL(triggered()),this, SLOT(OnSelectMode()));

	m_panselAction = new QAction("Pan and Selection mode",m_CoMenu);
	m_panselAction->setCheckable(TRUE);
	connect(m_panselAction, SIGNAL(triggered()),this, SLOT(OnPanSelectMode()));

	m_modeGroup = new QActionGroup(this);
	m_modeGroup->addAction(m_panAction);
	m_modeGroup->addAction(m_selAction);
	m_modeGroup->addAction(m_panselAction);
	m_panselAction->setChecked(true);




	m_setBGBlackAction = new QAction("Set Background Black",m_CoMenu);
	m_setBGBlackAction->setCheckable(TRUE);
	connect(m_setBGBlackAction, SIGNAL(triggered()),this, SLOT(OnChangeBKColorToBlack()));

	m_setBGWhiteAction = new QAction("Set Background White",m_CoMenu);
	m_setBGWhiteAction->setCheckable(TRUE);
	connect(m_setBGWhiteAction, SIGNAL(triggered()),this, SLOT(OnChangeBKColorToWhite()));

	m_BKGroup = new QActionGroup(this);
	m_BKGroup->addAction(m_setBGBlackAction);
	m_BKGroup->addAction(m_setBGWhiteAction);
	m_setBGBlackAction->setChecked(true);



	m_setViewFrontAction = new QAction("Front View",m_CoMenu);
	connect(m_setViewFrontAction, SIGNAL(triggered()),this, SLOT(OnFrontView()));

	m_setViewRightAction = new QAction("Right View",m_CoMenu);
	connect(m_setViewRightAction, SIGNAL(triggered()),this, SLOT(OnRightView()));

	m_setViewUpAction = new QAction("Up View",m_CoMenu);
	connect(m_setViewUpAction, SIGNAL(triggered()),this, SLOT(OnUpView()));






	m_hideSpheresAction = new QAction("Hide/Show Spheres",m_CoMenu);
	m_hideSpheresAction->setCheckable(TRUE);
	connect(m_hideSpheresAction, SIGNAL(triggered()),this, SLOT(OnHideSpheres()));
	m_hideSpheresAction->setChecked(true);

	m_hideLabelsAction = new QAction("Hide/Show Labels",m_CoMenu);
	m_hideLabelsAction->setCheckable(TRUE);
	connect(m_hideLabelsAction, SIGNAL(triggered()),this, SLOT(OnHideLabels()));
	m_hideLabelsAction->setChecked(true);



	m_polylineAction = new QAction("Start/Stop Polyline",m_CoMenu);
	m_polylineAction ->setCheckable(TRUE);
	connect(m_polylineAction, SIGNAL(triggered()),this, SLOT(OnStartStopPolyline()));
	m_polylineAction->setChecked(false);



	
	m_CoMenu->addAction(m_Update);
	m_CoMenu->addSeparator();
	m_CoMenu->addAction(m_panselAction);
	m_CoMenu->addAction(m_panAction);
	m_CoMenu->addAction(m_selAction);
	m_CoMenu->addSeparator();
	m_CoMenu->addAction(m_polylineAction);
	m_CoMenu->addSeparator();
	m_CoMenu->addAction(m_setBGBlackAction);
	m_CoMenu->addAction(m_setBGWhiteAction);
	m_CoMenu->addSeparator();
	m_CoMenu->addAction(m_setViewFrontAction);
	m_CoMenu->addAction(m_setViewRightAction);
	m_CoMenu->addAction(m_setViewUpAction);
	m_CoMenu->addSeparator();
	m_CoMenu->addAction(m_hideSpheresAction);
	m_CoMenu->addAction(m_hideLabelsAction);


	m_SpCoMenu = new QMenu(0);

	m_DeleteAction = new QAction("Delete All",m_SpCoMenu);
	connect(m_DeleteAction , SIGNAL(triggered()),this, SLOT(OnDeleteNode()));

	m_SpCoMenu->addAction(m_Update);
	m_SpCoMenu->addSeparator();
	m_SpCoMenu->addAction(m_DeleteAction);
	m_SpCoMenu->addSeparator();
	m_SpCoMenu->addAction(m_polylineAction);
	m_SpCoMenu->addSeparator();
	m_SpCoMenu->addAction(m_panselAction);
	m_SpCoMenu->addAction(m_panAction);
	m_SpCoMenu->addAction(m_selAction);
	m_SpCoMenu->addSeparator();
	m_SpCoMenu->addAction(m_setBGBlackAction);
	m_SpCoMenu->addAction(m_setBGWhiteAction);
	m_SpCoMenu->addSeparator();
	m_SpCoMenu->addAction(m_setViewFrontAction);
	m_SpCoMenu->addAction(m_setViewRightAction);
	m_SpCoMenu->addAction(m_setViewUpAction);
	m_SpCoMenu->addSeparator();
	m_SpCoMenu->addAction(m_hideSpheresAction);
	m_SpCoMenu->addAction(m_hideLabelsAction);
}
#pragma endregion

#pragma region "Ovel(float x,float y,float z,SbString text)"
SoSeparator *CNetworkWidget::CreateEllipseSeporator(float x,float y,float z,SbString text)
{
	c_viewingSt = FALSE;

	SoSeparator *root = new SoSeparator;
	SoLOD *lod = new SoLOD; 
	root->addChild(lod);
	lod->range.set1Value(0, 500);
	lod->range.set1Value(1, 1000);
	lod->range.set1Value(2, 1500);
	SoSeparator *level3 = new SoSeparator;
	SoSeparator *level2 = new SoSeparator;
	level2->insertChild(level3,0);
	if (m_hideSpheresAction->isChecked())
	{
		SoScale *scale = new SoScale;
		scale->scaleFactor.setValue(x,y,z);
		level2->addChild(scale);
		Material = new SoMaterial;
		//Material->shininess.setValue(.2);
		Material->transparency.set1Value(0,100);
		Material->diffuseColor.setValue(100,0,0);
		//Material->ambientColor.setValue(100,0,0);
		//Material->emissiveColor.setValue(100,0,0);
		level2->addChild(Material);
		SoSphere *sphere = new SoSphere;
		sphere->radius.setValue(2);
		level2->addChild(sphere);
	}
	SoSeparator *level1 = new SoSeparator;
	level1->insertChild(level2,0);
	if (m_hideLabelsAction->isChecked())
	{
		SoFont* font = new SoFont;
		font->size.setValue(8);
		level1->addChild(font);

		SoMaterial* Material2 = new SoMaterial;
		Material2->diffuseColor.setValue(50,50,0);
		level1->addChild(Material2);

		SoTransform* Transform2 = new SoTransform();
		Transform2->translation.setValue(0,-7,0);
		level1->addChild(Transform2);

		SoText2 * text2 = new SoText2;
		text2->string.setValue(text);
		level1->addChild(text2);
	}
	lod->addChild(level1);
	lod->addChild(level2);
	lod->addChild(level3);
	return root;
}
#pragma endregion

#pragma region "Mouse Event Handlers"

#pragma region "mouseMoveEvent ( QMouseEvent * event )"
void CNetworkWidget::mouseMoveEvent ( QMouseEvent * event )
{
}
#pragma endregion

#pragma region "mousePressEvent ( QMouseEvent * event )"
void CNetworkWidget::mousePressEvent ( QMouseEvent * event )
{

}
#pragma endregion

#pragma region "mouseReleaseEvent ( QMouseEvent * event )"
void CNetworkWidget::mouseReleaseEvent ( QMouseEvent * event )
{
	if (!c_panSt && !c_selSt)
	{
		//if(event->button() == Qt::LeftButton)
		//c_pViewer->setViewing(FALSE);
	}

	
}
#pragma endregion

#pragma region "wheelEvent ( QWheelEvent * event)"
void CNetworkWidget::wheelEvent ( QWheelEvent * event)
{
	/*if (!c_panSt && !c_selSt)
	{
		viewer->setViewing(TRUE);
		c_wheelSt = TRUE;
	}*/

}
#pragma endregion

#pragma endregion

#pragma region "Event Handlers "

#pragma region "void CNetworkWidget::OnPanMode()"
void CNetworkWidget::OnPanMode()
{
	c_selSt = FALSE;
	c_panSt = TRUE;

	c_pViewer->setViewing(TRUE);
}
#pragma endregion

#pragma region "void CNetworkWidget::OnSelectMode()"
void CNetworkWidget::OnSelectMode()
{
	c_selSt = TRUE;
	c_panSt = FALSE;

	c_pViewer->setViewing(FALSE);
}
#pragma endregion

#pragma region "void CNetworkWidget::OnPanSelectMode()"
void CNetworkWidget::OnPanSelectMode()
{
	c_selSt = FALSE;
	c_panSt = FALSE;
}
#pragma endregion

#pragma region "void CNetworkWidget::OnDeleteNode()"
void CNetworkWidget::OnDeleteNode()
{
	m_pRoot->removeAllChildren();
	//SoSeparator* sep1 = new SoSeparator;
	//SoSeparator* sep2 = new SoSeparator;

	//SoCamera* cam = viewer->getCamera();

	//SoCone* sp = new SoCone;
	//sp->height.setValue(25);
	//sp->bottomRadius.setValue(25);

	//SbVec3f *nodes = new SbVec3f[6];
	//nodes[0].setValue(30 ,0 ,0 );
	//nodes[1].setValue(-30,0 ,0 );
	//nodes[2].setValue(0 ,30 ,0 );
	//nodes[3].setValue(0 ,-30,0 );
	//nodes[4].setValue(0 ,0 ,30 );
	//nodes[5].setValue(0 ,0 ,-30);

	//SoCoordinate3 *Coordinate = new SoCoordinate3;
	//Coordinate->point.setValues(0,6,nodes);
	//sep1->addChild(Coordinate);

	//int a[3] = {2,2,2};

	//SoLineSet * line = new SoLineSet;
	//line->numVertices.setValues(0,3,a);




	//SoTransform  *a6 = new SoTransform ;
	//a6->translation.setValue(SbVec3f(-50,-50,-200));
	//a6->rotation.connectFrom(&cam->orientation);

	//SoTransform  *a7 = new SoTransform ;
	//a7->rotation.setValue(0,200,0,0);

	//SoTransform  *a8 = new SoTransform ;
	//a8->rotation.setValue(200,0,0,0);

	////sep1->addChild(a7);
	////sep1->addChild(a8);
	//sep1->addChild(a6);
	//sep1->addChild(line);
	//sep1->addChild(sp);


	//SoTransform  *hey = new SoTransform ;
	//hey->rotation.connectFrom(&cam->orientation);
	//hey->translation.connectFrom(&cam->position);

	//sep2->addChild(hey);
	//sep2->addChild(sep1);
	//root->addChild(sep2);
}
#pragma endregion

#pragma region "void CNetworkWidget::Update()"
void CNetworkWidget::Update(Path* path, int NodeID)
{
	if(!m_pMainWindow)
		return;

	if(NodeID == 0)
	{

		m_pRoot->removeAllChildren();
		root->removeAllChildren();


		int W=path->m_Carrier->Width;
		int H=path->m_Carrier->Length;
		int D=path->m_Carrier->Height;

		W*=1.1;
		H*=1.1;
		D*=1.1;

		float a = W;
		float b1 = H;
		float c = D;

		float focalDistance = sqrt(a*a+b1*b1+c*c);

		root->addChild(AddCubeNode(0,0,0,W,focalDistance/1000,focalDistance/1000,1,0,0));
		root->addChild(AddCubeNode(0,H,0,W,focalDistance/1000,focalDistance/1000,1,0,0));
		root->addChild(AddCubeNode(0,0,D,W,focalDistance/1000,focalDistance/1000,1,0,0));
		root->addChild(AddCubeNode(0,H,D,W,focalDistance/1000,focalDistance/1000,1,0,0));

		root->addChild(AddCubeNode(0,0,0,focalDistance/1000,H,focalDistance/1000,0,1,0));
		root->addChild(AddCubeNode(W,0,0,focalDistance/1000,H,focalDistance/1000,0,1,0));
		root->addChild(AddCubeNode(0,0,D,focalDistance/1000,H,focalDistance/1000,0,1,0));
		root->addChild(AddCubeNode(W,0,D,focalDistance/1000,H,focalDistance/1000,0,1,0));

		root->addChild(AddCubeNode(0,0,0,focalDistance/1000,focalDistance/1000,D,0,0,1));
		root->addChild(AddCubeNode(W,0,0,focalDistance/1000,focalDistance/1000,D,0,0,1));
		root->addChild(AddCubeNode(0,H,0,focalDistance/1000,focalDistance/1000,D,0,0,1));
		root->addChild(AddCubeNode(W,H,0,focalDistance/1000,focalDistance/1000,D,0,0,1));

		root->addChild(AddCubeNode(-0.3,-0.3,-0.3,W,H,0.3,0,0,1));





		for(int j=0;j<path->m_Carrier->m_BoxList.count();j++)
		{
			__BoxInContainer *Temp = path->m_Carrier->m_BoxList.at(j);
			
			Node * TempNode = getNode(Temp->NodeId);

			float r = 1;
			float g = 1;
			float b = 1;

			if(TempNode!=NULL)
			{
				__BoxRow* Box = GetBox(TempNode->m_Order->BoxID);
				if(Box != NULL)
				{
					QString Color =	Box->Color;
					QStringList rgbList = Color.split(" ");
					r = rgbList.at(0).toFloat();
					g = rgbList.at(1).toFloat();
					b = rgbList.at(2).toFloat();
				}
			}


			int x=Temp->x;
			int y=Temp->y;
			int z=Temp->z;

			int w=Temp->w;
			int h=Temp->h;
			int d=Temp->d;

			root->addChild(AddCubeNode((1.1*x),1.1*y,1.1*z,w,h,d,r,g,b));
		}	


		pViewer->getCamera()->position.setValue(path->m_Carrier->Width/2,path->m_Carrier->Height/2,focalDistance);
		pViewer->getCamera()->orientation.setValue(0,0,1,1);
		pViewer->getCamera()->focalDistance.setValue(focalDistance);

	}
	else
	{

		m_pRoot->removeAllChildren();
		root->removeAllChildren();


		int W=path->m_Carrier->Width;
		int H=path->m_Carrier->Length;
		int D=path->m_Carrier->Height;

		W*=1.1;
		H*=1.1;
		D*=1.1;

		float a = W;
		float b1 = H;
		float c = D;

		float focalDistance = sqrt(a*a+b1*b1+c*c);

		root->addChild(AddCubeNode(0,0,0,W,focalDistance/1000,focalDistance/1000,1,0,0));
		root->addChild(AddCubeNode(0,H,0,W,focalDistance/1000,focalDistance/1000,1,0,0));
		root->addChild(AddCubeNode(0,0,D,W,focalDistance/1000,focalDistance/1000,1,0,0));
		root->addChild(AddCubeNode(0,H,D,W,focalDistance/1000,focalDistance/1000,1,0,0));

		root->addChild(AddCubeNode(0,0,0,focalDistance/1000,H,focalDistance/1000,0,1,0));
		root->addChild(AddCubeNode(W,0,0,focalDistance/1000,H,focalDistance/1000,0,1,0));
		root->addChild(AddCubeNode(0,0,D,focalDistance/1000,H,focalDistance/1000,0,1,0));
		root->addChild(AddCubeNode(W,0,D,focalDistance/1000,H,focalDistance/1000,0,1,0));

		root->addChild(AddCubeNode(0,0,0,focalDistance/1000,focalDistance/1000,D,0,0,1));
		root->addChild(AddCubeNode(W,0,0,focalDistance/1000,focalDistance/1000,D,0,0,1));
		root->addChild(AddCubeNode(0,H,0,focalDistance/1000,focalDistance/1000,D,0,0,1));
		root->addChild(AddCubeNode(W,H,0,focalDistance/1000,focalDistance/1000,D,0,0,1));

		root->addChild(AddCubeNode(-0.1,-0.1,-0.1,W,H,0.1,0,0,1));




		for(int j=0;j<path->m_Carrier->m_BoxList.count();j++)
		{
			if(path->m_Carrier->m_BoxList.at(j)->NodeId==NodeID)
			{
				__BoxInContainer *Temp = path->m_Carrier->m_BoxList.at(j);

				Node * TempNode = getNode(Temp->NodeId);

				float r = 1;
				float g = 1;
				float b = 1;

				if(TempNode!=NULL)
				{
					__BoxRow* Box = GetBox(TempNode->m_Order->BoxID);
					if(Box != NULL)
					{
						QString Color =	Box->Color;
						QStringList rgbList = Color.split(" ");
						r = rgbList.at(0).toFloat();
						g = rgbList.at(1).toFloat();
						b = rgbList.at(2).toFloat();
					}
				}

				int x=Temp->x;
				int y=Temp->y;
				int z=Temp->z;

				int w=Temp->w;
				int h=Temp->h;
				int d=Temp->d;

				root->addChild(AddCubeNode((1.1*x),1.1*y,1.1*z,w,h,d,r,g,b));
			}
			else
			{
				__BoxInContainer *Temp = path->m_Carrier->m_BoxList.at(j);

				Node * TempNode = getNode(Temp->NodeId);

				float r = 1;
				float g = 1;
				float b = 1;

				if(TempNode!=NULL)
				{
					__BoxRow* Box = GetBox(TempNode->m_Order->BoxID);
					if(Box != NULL)
					{
						QString Color =	Box->Color;
						QStringList rgbList = Color.split(" ");
						r = rgbList.at(0).toFloat();
						g = rgbList.at(1).toFloat();
						b = rgbList.at(2).toFloat();
					}
				}

				int x=Temp->x;
				int y=Temp->y;
				int z=Temp->z;

				int w=Temp->w;
				int h=Temp->h;
				int d=Temp->d;

				root->addChild(AddCubeNode((1.1*x),1.1*y,1.1*z,w,h,d,r,g,b,0.5));
			}
		}	


	}
}
#pragma endregion

#pragma region "void CNetworkWidget::OnChangeBKColorToBlack()"
void CNetworkWidget::OnChangeBKColorToBlack()
{
	c_pViewer->setBackgroundColor(SbColor(0,0,0));
}
#pragma endregion

#pragma region "void CNetworkWidget::OnChangeBKColorToWhite()"
void CNetworkWidget::OnChangeBKColorToWhite()
{
	c_pViewer->setBackgroundColor(SbColor(100,100,100));
}
#pragma endregion

#pragma region "void CNetworkWidget::OnFrontView()"
void CNetworkWidget::OnFrontView()
{
	c_pViewer->getCamera()->position.setValue(0,0,10000);
	c_pViewer->getCamera()->orientation.setValue(0,0,1,1);
	c_pViewer->getCamera()->focalDistance.setValue(10000);
}
#pragma endregion

#pragma region "void CNetworkWidget::OnRightView()"
void CNetworkWidget::OnRightView()
{
	c_pViewer->getCamera()->position.setValue(10000,0,0);
	c_pViewer->getCamera()->focalDistance.setValue(10000);
	c_pViewer->getCamera()->orientation.setValue(0, 1, 0, 1);
}
#pragma endregion

#pragma region "void CNetworkWidget::OnUpView()"
void CNetworkWidget::OnUpView()
{
	c_pViewer->getCamera()->position.setValue(0,10000,0);
	c_pViewer->getCamera()->orientation.setValue(-1,0,0,1);
	c_pViewer->getCamera()->focalDistance.setValue(10000);
}
#pragma endregion

#pragma region "void CNetworkWidget::OnHideSpheres()"
void CNetworkWidget::OnHideSpheres()
{
	//Update();
}
#pragma endregion

#pragma region "void CNetworkWidget::OnHideLabels()"
void CNetworkWidget::OnHideLabels()
{
	//Update();
}
#pragma endregion

#pragma region "void CNetworkWidget::OnShowAll()"
void CNetworkWidget::OnShowAll()
{
}
#pragma endregion

#pragma region "void CNetworkWidget::OnStartStopPolyline()"
void CNetworkWidget::OnStartStopPolyline()
{
	if(!m_polylineAction->isChecked())
		m_LastPickedPoint = SbVec3f(0,0,0);
}
#pragma endregion

#pragma endregion

#pragma region "SoSeparator*	CNetworkWidget::AddCubeNode(float x,float y ,float z,float w,float h,float d)"
SoSeparator*	CNetworkWidget::AddCubeNode(float x,float y ,float z,float w,float h,float d,float r,float g,float b , float TR)
{
	SoSeparator * child = new SoSeparator;
	SoCube* pCube=new SoCube;
	SoTransform * pTrans=new SoTransform;
	pTrans->translation.setValue(x+w/2.0,y+h/2.0,z+d/2.0);
	pCube->width=w;
	pCube->height=h;
	pCube->depth=d;

	SoMaterial* myMaterial =new SoMaterial;
	myMaterial->diffuseColor.setValue(r,g,b);
	myMaterial->transparency.setValue(TR);
	child->addChild(myMaterial);

	child->addChild(pTrans);
	child->addChild(pCube);
	return child;
}
#pragma endregion

#pragma region "SoSeparator*	CNetworkWidget::AddCubeNode(float x,float y ,float z,float w,float h,float d)"
Node*	CNetworkWidget::getNode	(int NodeID)
{
	for(int i = 0 ; i < m_pProject->m_AllPathes.count() ; i++)
	{
		Path* tempPath = m_pProject->m_AllPathes.at(i);
		for(int j = 0 ; j < tempPath->m_Node.count() ; j++)
		{
			Node* tempNode = tempPath->m_Node.at(j);
			if(tempNode->id == NodeID)
				return tempNode;
		}

	}
	return NULL;
}
#pragma endregion

#pragma region "__BoxRow* CNetworkWidget::GetBox(int id)" 
__BoxRow* CNetworkWidget::GetBox(int id)
{
	for(int i=0;i<m_pProject->m_CargoList.count();i++)
	{
		__BoxRow* box=m_pProject->m_CargoList.at(i);
		if(box->id==id)
			return box;
	}
	return NULL;
}
#pragma endregion
