#ifndef _MAINMENUSCENE_H
#define _MAINMENUSCENE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KActor.h"
#include "KGenCardPanel.h"
#include "KGameResultPanel.h"
#include "../PlayerCard/KPlayerCardDepot.h"
#include "KGameSettingPanel.h"

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
	void ShowCollectInfo();
	void DoClickTiebar(CCObject* sender);
	void DoClickGameCenter(CCObject* sender);
	void SetRaceCollectInfo(KItemUnitList& tmpList,KCardStatic::CardRace race);

	void DoClickGameSetting(CCObject* sender);
private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;
	void InitTest();
	void UpdateLockStatus(const char* key,const char* butName,const char* lockImage);
	void runAutoTest(float dt);

	KGameSettingPanel m_SettingPanel;
};

#endif // __MainMenuScene_SCENE_H__
