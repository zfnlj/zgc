#ifndef _KSELECTCARDPANEL_H
#define _KSELECTCARDPANEL_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;
class FBattleGuy;
class KSelectBeginCardPanel :public CCObject
{
public:
	KSelectBeginCardPanel();
	~KSelectBeginCardPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickOK(CCObject* sender);
	void onSelectCardOK(FBattleGuy* guy);

	void onClickCard(CCObject* sender);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_ui;
};

#endif // __BattleFieldScene_SCENE_H__
