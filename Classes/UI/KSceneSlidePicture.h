#ifndef _KSCENESLIDEPICTURE_H
#define _KSCENESLIDEPICTURE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "assist/KRadioWidget.h"

USING_NS_CC;

class KSceneSlidePicture :public cocos2d::extension::UILayer
{
public:
	KSceneSlidePicture(){}
	~KSceneSlidePicture();
	
	virtual void update(float dt);
	virtual bool init();
	void CreateCloseBut();
	void menuCloseCallback(CCObject* pSender);
	virtual void onCloseCallback()=0;
	void onClickNext();
	void CreateSelectRadio();
protected:
	cocos2d::extension::UIWidget* m_ui;

	KRadioWidget m_radioSelect;
};

#endif // __BattleFieldScene_SCENE_H__
