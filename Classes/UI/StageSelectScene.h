#ifndef _KSTAGESELECTSCENE_H
#define _KSTAGESELECTSCENE_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;

class KHelpStringStatic;
class StageSelectScene : public cocos2d::CCLayer
{
public:
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(StageSelectScene);

	cocos2d::extension::UILayer* GetUILayer();
	cocos2d::extension::UILayer* GetLayer(){ return m_ui;}

	void DoClickClose(CCObject* sender);
	void DoClickStage(CCObject* sender);
	void DoClickTower(CCObject* sender);
	void InitStageBut();
	void InitTower();
private:
	cocos2d::extension::UILayer* m_ui;
};

#endif // __StageSelectScene_SCENE_H__
