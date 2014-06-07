#ifndef _RECCARDGROUPSCENE_H
#define _RECCARDGROUPSCENE_H

#include "CardGroupScene.h"


USING_NS_CC;
using namespace cocos2d::extension;

class RecCardGroupScene : public CardGroupScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(RecCardGroupScene);

	virtual bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
protected:
	void onClickBack(CCObject* sender);
	void onClickSlot(CCObject* sender);
	void onClickNewCardGroup(CCObject* sender);
	void onClickMiniCard(CCObject* obj);
	void onClickMiniHero(CCObject* obj);
	void onClickSlotAdd(CCObject* sender);
	void onClickSlotSub(CCObject* sender);
	void onClickCost(CCObject* sender);
	void onClickMainType(CCObject* sender);
	void onClickPageDown(CCObject* sender);
	void onClickPageUp(CCObject* sender);
	void onClickRace(CCObject* sender);
	void onClickSelectHero(CCObject* sender);
	void onClickCard(CCObject* sender);
	void DoClearDeck(CCObject* sender);
	void onClickSaveDeck(CCObject* sender);
	void onClickClearDeck(CCObject* sender);
};

#endif // __CardGroupScene_SCENE_H__
