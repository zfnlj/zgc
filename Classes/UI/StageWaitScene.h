#ifndef _KSTAGEWAITSCENE_H
#define _KSTAGEWAITSCENE_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;

class StageWaitScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(StageWaitScene);

	cocos2d::extension::UILayer* GetUILayer();
	cocos2d::extension::UILayer* GetLayer(){ return m_ui;}

	void DoClickClose(CCObject* sender);
private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;

};

#endif // __StageWaitScene_SCENE_H__
