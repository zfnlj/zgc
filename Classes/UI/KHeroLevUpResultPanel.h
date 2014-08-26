#ifndef _KHEROLEVUPRESULT_H
#define _KHEROLEVUPRESULT_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"
#include "../GameLogic/KGameDef.h"

USING_NS_CC;
using namespace cocos2d::extension;

class KHeroLevUpResultPanel :public CCObject
{
public:
	KHeroLevUpResultPanel();
	~KHeroLevUpResultPanel();

	void init(cocos2d::extension::UILayer* layer);
	void update(float dt);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
};

#endif 
