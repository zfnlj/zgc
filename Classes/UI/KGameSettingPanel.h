#ifndef _KGAMESETTINGPANEL_H
#define _KGAMESETTINGPANEL_H

#include "cocos2d.h"
#include "cocos-ext.h"
#include "UILayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

class KGameSettingPanel :public CCObject
{
public:
	KGameSettingPanel();
	~KGameSettingPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void ShowPanel();

	void DoClickSoundCheck(CCObject* sender);
	void DoClickSoundVol(CCObject *pSender, SliderEventType type);
	void DoClickMusicCheck(CCObject* sender);
	void DoClickMusicVol(CCObject *pSender, SliderEventType type);
	bool IsShow();
	void UpdatePanel();
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;
	CCPoint m_pt;
};

#endif // __BattleFieldScene_SCENE_H__
