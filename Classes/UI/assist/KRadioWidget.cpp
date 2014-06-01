#include "KRadioWidget.h"

void KRadioBut::onClick()
{
	if (m_pPushListener && m_pfnPushSelector)
	{
		(m_pPushListener->*m_pfnPushSelector)(m_pBut);
	}
}

void KRadioWidget::AddBut(UICheckBox* pBut,CCObject* target,SEL_PushEvent selector)
{
	m_butList.push_back(KRadioBut(pBut,target,selector));
	pBut->addPushDownEvent(this, coco_pushselector(KRadioWidget::onClick));
}

void KRadioWidget::onClick(CCObject* obj)
{
	m_pSelected = obj;
	for(KRadioButList::iterator it=m_butList.begin();it!=m_butList.end();++it){
		KRadioBut& radioBut =*it;
		if(radioBut.m_pBut==obj){
			radioBut.m_pBut->setTouchEnable(false);
			radioBut.onClick();
		}else{
			radioBut.m_pBut->setTouchEnable(true);
			radioBut.m_pBut->setSelectedState(false);
		}
	}
}

void KRadioWidget::SetVisible(bool flag)
{
	for(KRadioButList::iterator it=m_butList.begin();it!=m_butList.end();++it){
		KRadioBut& radioBut =*it;
		radioBut.m_pBut->setVisible(flag);
		radioBut.m_pBut->setTouchEnable(flag);
		if(flag && radioBut.m_pBut==m_pSelected) radioBut.m_pBut->setTouchEnable(false);
	}
}