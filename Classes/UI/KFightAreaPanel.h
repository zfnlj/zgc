#ifndef _KFIGHTAREAPANEL_H
#define _KFIGHTAREAPANEL_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KActor.h"
#include "../GameLogic/KBattleDeck.h"
USING_NS_CC;
class FBattleGuy;
class KFightAreaPanel :public CCObject
{
public:
	KFightAreaPanel();
	~KFightAreaPanel();

	void init(cocos2d::extension::UILayer* layer);
	virtual void update(float);
	void Show();
	void Hide();
private:
	void ActiveSlot(int pos,bool flag);
	cocos2d::extension::UILayer* m_layer;
	void onClickFightArea(CCObject* sender);
	KActor m_area;
	bool m_bShowActive;
	CCSprite* m_ActiveSprites[MAX_FIGHT_POS_NUM];
};

#endif // __BattleFieldScene_SCENE_H__
