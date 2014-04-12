#ifndef _KRESPANEL_H
#define _KRESPANEL_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;
class FBattleGuy;
class KResPanel :public CCObject
{
public:
	KResPanel();
	~KResPanel(){}

	void init(cocos2d::extension::UILayer* layer);
	void UpdateRes();
private:
	cocos2d::extension::UILayer* m_layer;
	CCLabelBMFont* m_myResFont;
	CCLabelBMFont* m_yourResFont;
};

#endif // __BattleFieldScene_SCENE_H__
