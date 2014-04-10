#ifndef _MAINMENUSCENE_H
#define _MAINMENUSCENE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KActor.h"
#include "KShopPanel.h"
#include "KGenCardPanel.h"
#include "KMyStorePanel.h"
#include "KMyQuestPanel.h"
#include "KGameResultPanel.h"

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
    void menuCloseCallback(CCObject* pSender);
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(MainMenuScene);

	CCArray m_testArr;
	CCPoint GetCenterPos();

	cocos2d::extension::UILayer* GetUILayer();
	void DoClickShopBut(CCObject* sender);
	void DoClickBattleBut(CCObject* sender);
	void DoClickQuestBut(CCObject* sender);
	void DoClickStoreBut(CCObject* sender);

	void onGenPlayerCard(unsigned long long p1);
	void openMyStorePanel();

	void OnSelectCard(KCardActor*);
	void onUpdateMoney();
	void onQuestUpdate();
	void onUpdateBag();
	void onQuestFinished(int qId);
	cocos2d::extension::UILayer* GetLayer(){ return m_ui;}
private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;
	void InitTest();

	KShopPanel	m_shopPanel;
	KGenCardPanel m_genCardPanel;
	KMyStorePanel m_myStorePanel;
	KMyQuestPanel m_myQuestPanel;
	KGameResultPanel m_gameResultPanel;
};

#endif // __MainMenuScene_SCENE_H__
