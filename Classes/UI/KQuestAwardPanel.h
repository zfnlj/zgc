#ifndef _KQUESTAWARDPANEL_H
#define _KQUESTAWARDPANEL_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;

#define MAX_SLOT_NUM 8


class KQuestAwardPanel :public CCObject
{
public:
	KQuestAwardPanel();
	~KQuestAwardPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;

};

#endif // __BattleFieldScene_SCENE_H__
