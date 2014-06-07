#ifndef _KGAMERECORDPANEL_H
#define _KGAMERECORDPANEL_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;

class KGameRecordPanel :public CCObject
{
public:
	KGameRecordPanel();
	~KGameRecordPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickPlay(CCObject* sender);
	void DoClickRecord(CCObject* sender);
	void DoClickStop(CCObject* sender);
private:
	cocos2d::extension::UIWidget* m_panel;
};

#endif // __BattleFieldScene_SCENE_H__
