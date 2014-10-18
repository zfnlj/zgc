#ifndef _KSCENESLIDEPICTURE_H
#define _KSCENESLIDEPICTURE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "assist/KRadioWidget.h"

USING_NS_CC;

class KSceneSlidePicture :public cocos2d::CCLayer
{
public:
	KSceneSlidePicture(){}
	~KSceneSlidePicture();
	
	CREATE_FUNC(KSceneSlidePicture);

	virtual void update(float dt);
	virtual bool init();
	static cocos2d::CCScene* scene();
	void CreateCloseBut();
	void menuCloseCallback(CCObject* pSender);
	void onClickNext(CCObject* sender);
	void onClickSelectSlide(CCObject* sender);
	void OpenSceneSlide(int first,int last);
	void UpdatePanel();
	static void Setting(int first,int last){
		m_firstId = m_curId = first;
		m_lastId = last;
	}
protected:
	void CreateCheckSlideRadio();
	cocos2d::extension::UIWidget* m_ui;

	KRadioWidget m_radioSelect;

	static int m_firstId;
	static int m_lastId;
	static int m_curId;
};

#endif // __BattleFieldScene_SCENE_H__
