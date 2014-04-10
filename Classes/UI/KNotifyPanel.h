#ifndef _KNOTIFYPANEL_H
#define _KNOTIFYPANEL_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KActor.h"

USING_NS_CC;
class FBattleGuy;
class KNotifyPanel :public CCObject
{
public:
	KNotifyPanel();
	~KNotifyPanel(){}

	void init(cocos2d::extension::UIWidget* notify);
	void breathe(float dt);
	void onTurnBegin(bool myTurn);
private:
	cocos2d::extension::UIWidget* m_notify;
	KActor m_turnActor;
};

#endif // __BattleFieldScene_SCENE_H__
