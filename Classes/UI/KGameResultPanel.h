#ifndef _KGAMERESULT_H
#define _KGAMERESULT_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"
#include "../GameLogic/KGameDef.h"
USING_NS_CC;
using namespace cocos2d::extension;
#define MAX_QUEST_SLOT_NUM 3

class KQuestNew;
class KGameResultPanel :public CCObject
{
public:
	enum result_type{
		result_win,
		result_lost,
		result_equal,
	};
	KGameResultPanel();
	~KGameResultPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void DoClickContinue(CCObject* sender);
	void updatePanel();
	void ShowPanel();

	void onGameEnd(unsigned long long Param1);
	void onQuestPreOver(KQuestNew* pQuest);
private:
	bool ShowSelectGift(KQuestNew* pQuest);
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
	void DoClickSlot(CCObject* sender);
	CCPoint m_pt;
	strGameResult m_result;
	result_type m_resultType;
	bool m_bSelectGift;
};

#endif // __BattleFieldScene_SCENE_H__
