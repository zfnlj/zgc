#ifndef _KRADIOWIDGET_H
#define _KRADIOWIDGET_H

#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


struct KRadioBut{
	UICheckBox*		m_pBut;
	CCObject*       m_pPushListener;
	SEL_PushEvent    m_pfnPushSelector;
	KRadioBut(UICheckBox* widget,CCObject* listener, SEL_PushEvent evt):m_pBut(widget),m_pPushListener(listener),m_pfnPushSelector(evt)
	{}
	void onClick();
};
typedef std::list<KRadioBut> KRadioButList;

class KRadioWidget :public CCObject
{
public:
	KRadioWidget(){ m_bUnSelectable =false;}
	~KRadioWidget(){}
	
	void AddBut(UICheckBox* pBut,CCObject* target,SEL_PushEvent selector,bool bCheck=false);
	void AddGroupBut(const char* name,int num,cocos2d::extension::UILayer*,CCObject* target,SEL_PushEvent selector,int checked=-1);
	void onClick(CCObject* obj);
	void SetVisible(bool flag);
	void SetVisible(int index, bool flag);
	void SetSelected(int index,bool bClick);
	int  GetSelectVal();
	bool GetSelectState(int& val,bool& bChecked);
	void SetUnSelectAble(bool flag){ m_bUnSelectable = flag;}
	void ClearSelected();
private:
	KRadioButList m_butList;
	CCObject* m_pSelected;
	bool m_bUnSelectable;

};

#endif // __HELLOWORLD_SCENE_H__	
