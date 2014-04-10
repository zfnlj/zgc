#ifndef _KGAMERESULT_H
#define _KGAMERESULT_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"
USING_NS_CC;
using namespace cocos2d::extension;
#define MAX_QUEST_SLOT_NUM 3

struct GameResult{
	GameResult():_money(0),_exp(0){}
	int _money;
	int _exp;
};

class KGameResultPanel :public CCObject
{
public:
	enum panel_type{
		enum_panel_task,
		enum_panel_game,
		enum_panel_null,
	};
	KGameResultPanel();
	~KGameResultPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void updatePanel();
	void ShowPanel(GameResult& result,panel_type tp=enum_panel_task);

	void onQuestFinished(int qId);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;

	CCPoint m_pt;
	panel_type m_panelType;
	GameResult m_result;
};

#endif // __BattleFieldScene_SCENE_H__
