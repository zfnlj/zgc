#ifndef _KSTAGESELECTSCENE_H
#define _KSTAGESELECTSCENE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KSceneLayerBase.h"

USING_NS_CC;

class KHelpStringStatic;
class StageSelectScene : public KSceneLayerBase
{
public:
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(StageSelectScene);

	void DoClickClose(CCObject* sender);
	void DoClickStage(CCObject* sender);
	void DoClickTower(CCObject* sender);
	void InitStageBut();
	void InitTower();
	virtual void onCloseCallback();
};

#endif // __StageSelectScene_SCENE_H__
