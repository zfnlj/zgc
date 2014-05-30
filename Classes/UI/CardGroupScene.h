#ifndef _CARDGROUPSCENE_H
#define _CARDGROUPSCENE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "assist/KCardGroupAssist.h"

USING_NS_CC;
class KCardInst;
class KCardInst;
class KPlayerCardDepot;
class CardGroupScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(CardGroupScene);

	cocos2d::extension::UILayer* GetUILayer();

private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;
	void InitTest();

	void CreateMiniCardList(int index);
	void ShowMiniCardList();
	KPlayerCardDepot* m_depot;
	CardGroupElemList m_miniCardList;
};

#endif // __CardGroupScene_SCENE_H__
