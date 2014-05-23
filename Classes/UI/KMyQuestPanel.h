#ifndef _KMYQUESTPANEL_H
#define _KMYQUESTPANEL_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"
USING_NS_CC;
using namespace cocos2d::extension;
#define MAX_QUEST_SLOT_NUM 3


class KMyQuestPanel :public CCObject
{
public:
	KMyQuestPanel();
	~KMyQuestPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void updatePanel();
private:
	void DoCancleQuest(CCObject* sender);
	void DoAcceptQuest(CCObject* sender);
	void DoClickSumbit(CCObject* sender);

	
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
};

#endif // __BattleFieldScene_SCENE_H__
