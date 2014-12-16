#ifndef _KGAMETIPPANEL_H
#define _KGAMETIPPANEL_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

class KGameTipPanel :public CCObject
{
public:
	KGameTipPanel();
	~KGameTipPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void ShowPanel();

	void UpdatePanel();
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
	CCPoint m_pt;
};

#endif // __BattleFieldScene_SCENE_H__
