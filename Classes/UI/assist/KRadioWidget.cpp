#include "KRadioWidget.h"

void KRadioBut::onClick()
{
	if (m_pPushListener && m_pfnPushSelector)
	{
		(m_pPushListener->*m_pfnPushSelector)(m_pBut);
	}
}

void KRadioWidget::AddGroupBut(const char* name,int num,cocos2d::extension::UILayer* layer,CCObject* target,SEL_PushEvent selector,int checked)
{
	for(int i=0;i<num;i++){
		char sz[32];
		sprintf(sz,"%s_%d",name,i);
		UICheckBox* pCheck = (UICheckBox*)layer->getWidgetByName(sz);
		pCheck->setTag(i);
		AddBut(pCheck,target,selector,(i==checked));
	}
}

void KRadioWidget::AddBut(UICheckBox* pBut,CCObject* target,SEL_PushEvent selector,bool bCheck)
{
	m_butList.push_back(KRadioBut(pBut,target,selector));
	pBut->setTouchEnable(true);
	pBut->addPushDownEvent(this, coco_pushselector(KRadioWidget::onClick));
	if(bCheck){
		pBut->setSelectedState(true);
		m_pSelected = pBut;
	}
}

bool KRadioWidget::GetSelectState(int& val,bool& bChecked)
{
	UICheckBox* pSelected = (UICheckBox*)m_pSelected;
	if(!pSelected) return false;
	bChecked = !pSelected->getSelectedState();//falst -> true
	val = pSelected->getTag();
	return true;
}

int  KRadioWidget::GetSelectVal()
{
	UIWidget* pSelected = (UIWidget*)m_pSelected;
	if(pSelected){
		return pSelected->getTag();
	}else{
		return -1;
	}
}

void KRadioWidget::SetSelected(int index,bool bClick)
{
	int pos=0;
	if(index<0) m_pSelected = NULL;
	for(KRadioButList::iterator it=m_butList.begin();it!=m_butList.end();++it,pos++){
		KRadioBut& radioBut =*it;
		if(pos==index){
			radioBut.m_pBut->setSelectedState(true);
			if(!m_bUnSelectable)radioBut.m_pBut->setTouchEnable(false);
			m_pSelected = radioBut.m_pBut;
			if(bClick){
				onClick(radioBut.m_pBut);
				return;
			}
		}else{
			radioBut.m_pBut->setSelectedState(false);
		}
	}
}

void KRadioWidget::onClick(CCObject* obj)
{
	m_pSelected = obj;
	for(KRadioButList::iterator it=m_butList.begin();it!=m_butList.end();++it){
		KRadioBut& radioBut =*it;
		if(radioBut.m_pBut==obj){
			if(!m_bUnSelectable)radioBut.m_pBut->setTouchEnable(false);
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
		if(flag && radioBut.m_pBut==m_pSelected && !m_bUnSelectable) radioBut.m_pBut->setTouchEnable(false);
	}
}

void KRadioWidget::SetVisible(int index, bool flag)
{
	int pos=0;
	for(KRadioButList::iterator it=m_butList.begin();it!=m_butList.end();++it,pos++){
		if(index == pos){
			KRadioBut& radioBut =*it;
			radioBut.m_pBut->setVisible(flag);
			radioBut.m_pBut->setTouchEnable(flag);
			if(flag && radioBut.m_pBut==m_pSelected && !m_bUnSelectable) radioBut.m_pBut->setTouchEnable(false);
			return;
		}
	}
}

void KRadioWidget::ClearSelected()
{
	if(m_pSelected) m_pSelected = NULL;
	for(KRadioButList::iterator it=m_butList.begin();it!=m_butList.end();++it){
		KRadioBut& radioBut =*it;
		radioBut.m_pBut->setSelectedState(false);
	}
}