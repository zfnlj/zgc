#ifndef _KHEROLEVUPRESULT_H
#define _KHEROLEVUPRESULT_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"
#include "../GameLogic/KGameDef.h"
#include "../PlayerCard/KPlayerDeck.h"

USING_NS_CC;
using namespace cocos2d::extension;
struct KHeroDef;
class KHeroLevUpResultPanel :public CCObject
{
public:
	KHeroLevUpResultPanel();
	~KHeroLevUpResultPanel();

	void init(cocos2d::extension::UILayer* layer);
	void update(float dt);
	void Show(const KHeroDef& oldHero,const KHeroDef& newHero);
	void DoClose(float dt);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
	float m_elapsed;
	KHeroDef m_oldHero;
	KHeroDef m_newHero;
	bool m_bPlay;
	const KHeroDef::skillDef* m_pNewSkill;
	
};

#endif 
