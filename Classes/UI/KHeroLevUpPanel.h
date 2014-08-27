#ifndef _KHEROLEVUPPANEL_H
#define _KHEROLEVUPPANEL_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"
#include "../GameLogic/KGameDef.h"
#include "KHeroLevUpResultPanel.h"

USING_NS_CC;
using namespace cocos2d::extension;

struct KHeroDef;
class KHeroLevUpPanel :public CCObject
{
public:
	KHeroLevUpPanel();
	~KHeroLevUpPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void updatePanel();
	void update(float dt);
	void ShowPanel(KHeroDef*);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
	KHeroDef* m_pHeroDef;
	UIWidget*	m_pHeroWidget;
	KHeroLevUpResultPanel m_resultPanel;
	void DoClickHeroLevUp(CCObject* sender);
	void SetLevUpWidgetsVisible(int index,bool bVisible);
	void UpdateHeroLevUpInfo();
};

#endif // _KHEROLEVUPPANEL_H
