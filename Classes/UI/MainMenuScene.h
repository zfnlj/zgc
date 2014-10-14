#ifndef _MAINMENUSCENE_H
#define _MAINMENUSCENE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KActor.h"
#include "KGenCardPanel.h"
#include "KGameResultPanel.h"
#include "../PlayerCard/KPlayerCardDepot.h"
USING_NS_CC;
class KCardInst;
class KCardInst;

class MainMenuScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuMailCallback(CCObject* pSender);
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(MainMenuScene);

	cocos2d::extension::UILayer* GetUILayer();
	void DoClickShopBut(CCObject* sender);
	void DoClickBattleBut(CCObject* sender);
	void DoClickAdventureBut(CCObject* sender);
	void DoClickStoreBut(CCObject* sender);
	void DoClickHeroBrowse(CCObject* sender);

	void OnSelectCard(KCardActor*);
	void onUpdateMoney();
	void onQuestUpdate();
	void onUpdateBag();
	void onQuestFinished(int qId);
	void UpdateLockStatus();
	cocos2d::extension::UILayer* GetLayer(){ return m_ui;}
	void DoClickFeedback(CCObject* sender);
	void ShowCollectInfo();

	void SetRaceCollectInfo(KItemUnitList& tmpList,KCardStatic::CardRace race);
private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;
	void InitTest();
	void UpdateLockStatus(const char* key,const char* butName,const char* lockImage);
	KGameResultPanel m_gameResultPanel;
	void runAutoTest(float dt);
};

#endif // __MainMenuScene_SCENE_H__
