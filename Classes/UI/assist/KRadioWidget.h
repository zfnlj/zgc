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
	KRadioWidget(){}
	~KRadioWidget(){}
	
	void AddBut(UICheckBox* pBut,CCObject* target,SEL_PushEvent selector);
	void onClick(CCObject* obj);
	void SetVisible(bool flag);
private:
	KRadioButList m_butList;
	CCObject* m_pSelected;

};

#endif // __HELLOWORLD_SCENE_H__	
