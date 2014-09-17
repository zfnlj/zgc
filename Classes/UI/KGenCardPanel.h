#ifndef _KGENCARDPANEL_H
#define _KGENCARDPANEL_H

#include "cocos2d.h"
#include "UILayer.h"
#include "../GameLogic/KCardInst.h"
#define  MAX_GEN_CARD_NUM 3

USING_NS_CC;
class FBattleGuy;
class KGenCardPanel :public CCObject
{
public:
	KGenCardPanel();
	~KGenCardPanel();

	void init(cocos2d::extension::UILayer* layer,unsigned long long p1);
	void DoClickOK(CCObject* sender);
	void onClickCard(CCObject* sender);
	virtual void update(float dt);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;

	KCardInst m_GenCard[MAX_GEN_CARD_NUM];
	float m_delayCloseTime;
};

#endif // __BattleFieldScene_SCENE_H__
