
#pragma region "Includes"
#include "ExaminerViewer.h"
#include "NetworkWidget.h"
#pragma endregion

#pragma region "Constructor"
CExaminerViewer::CExaminerViewer(QWidget *parent)
: SoQtExaminerViewer(parent)
{


}
#pragma endregion

#pragma region "Destructor"
CExaminerViewer::~CExaminerViewer()
{

}
#pragma endregion

#pragma region "setViewingSt(bool t)"
void CExaminerViewer::setViewingSt(bool t)
{
	CNetworkWidget::c_pViewer->setViewing(t);
}
#pragma endregion

#pragma region "panSt()"
bool CExaminerViewer::panSt()
{
	return CNetworkWidget::c_panSt;
}
#pragma endregion

#pragma region "selSt()"
bool CExaminerViewer::selSt()
{
	return CNetworkWidget::c_selSt;
}
#pragma endregion

#pragma region "Virtual processSoEvent(const SoEvent * const ev)"
SbBool CExaminerViewer::processSoEvent(const SoEvent * const ev)
{
	

	if (!CNetworkWidget::c_panSt && !CNetworkWidget::c_selSt)
	{

		if(CNetworkWidget::c_wheelSt)
		{
			CNetworkWidget::c_wheelSt = FALSE;
			this->setViewing(FALSE);
		}


		const SoType type(ev->getTypeId());

		const SbVec2s size(this->getGLSize());
		const SbVec2s pos(ev->getPosition());
		SbVec2s pos2;
		const SbVec2f posn((float) pos[0] / (float) SoQtMax((int)(size[0] - 1), 1),(float) pos[1] / (float) SoQtMax((int)(size[1] - 1), 1));


		SbBool processed = TRUE;

		if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) 
		{
			processed = FALSE;

			SoMouseButtonEvent * event = (SoMouseButtonEvent *) ev;
			const int button = event->getButton();
			const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;
			const SbVec2s pos3(ev->getPosition());



			switch (button) 
			{
			case SoMouseButtonEvent::BUTTON1:

				processed = TRUE;
				//pos2 = pos3;
				break;

			case SoMouseButtonEvent::BUTTON2:
				this->setViewing(FALSE);
				//event->setButton(SoMouseButtonEvent::BUTTON1);
				//SoQtExaminerViewer::processSoEvent(event);

				processed = TRUE;
				pos2 = pos3;
				break;

			case SoMouseButtonEvent::BUTTON3:
				break;

				default:
				processed = TRUE;
				break;
			}
		}

		return 0;

	}
	else if(CNetworkWidget::c_selSt)
	{
		const SoType type(ev->getTypeId());


		if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) 
		{

			SoMouseButtonEvent * event = (SoMouseButtonEvent *) ev;
			const int button = event->getButton();
			const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;



			switch (button) 
			{
			case SoMouseButtonEvent::BUTTON2:
				event->setButton(SoMouseButtonEvent::BUTTON1);
				return SoQtExaminerViewer::processSoEvent(event);
				break;
			}
		}
		SoQtExaminerViewer::processSoEvent(ev);
	}
	else
	{
		const SoType type(ev->getTypeId());


		if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) 
		{

			SoMouseButtonEvent * event = (SoMouseButtonEvent *) ev;
			const int button = event->getButton();
			const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;



			switch (button) 
			{
			case SoMouseButtonEvent::BUTTON2:
				this->setViewing(FALSE);
				event->setButton(SoMouseButtonEvent::BUTTON1);
				return SoQtExaminerViewer::processSoEvent(event);
				break;
			}
		}
		return SoQtExaminerViewer::processSoEvent(ev);
	}
	return true;
}
#pragma endregion


