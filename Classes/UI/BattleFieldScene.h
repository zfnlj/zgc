#ifndef __BATTLEFIELDSCENE__H__
#define __BATTLEFIELDSCENE__H__

#include "cocos2d.h"
#include "UILayer.h"
#include "KSelectBeginCardPanel.h"
#include "KResPanel.h"
#include "KIndicatePanel.h"
#include "KFightAreaPanel.h"
#include "KNotifyPanel.h"
#include "KActor.h"
#include "KShopPanel.h"
#include "KGenCardPanel.h"
#include "KMyStorePanel.h"
#include "KGameResultPanel.h"

USING_NS_CC;
class KCardInst;
struct strCardDuelResult;
struct strCardAbilityResult;
struct strSummonCardResult;
struct strReplaceCardResult;
class KCardInst;
class FBattleGuy;

class BattleFieldScene : public cocos2d::CCLayer
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
    CREATE_FUNC(BattleFieldScene);

	CCArray m_testArr;
	CCPoint GetCenterPos();

	KIndicatePanel& IndicatePanel(){ return m_indicatePanel;}
	cocos2d::extension::UILayer* GetUILayer();
	void onBattleInit();
	void onHandCardReady();
	void FreshAllCard();
	void onCardDuelResult(strCardDuelResult* result);
	void onSummonCard(strSummonCardResult* result);
	void onUseAbilityResult(strCardAbilityResult* result);
	void onSelectCardOK(FBattleGuy* guy);
	void onTurnBegin();
	void onTurnEnd();
	void onDrawCard(KCardInstList* cardList,bool bInit=true);
	void onCardMove(KCardInst* pCard);
	void onUseSecretCard(KCardInst* card);
	void onUseRes();

	void onGameEnd();
	void OnSelectSrcCard(KCardActor*);
	void DoEndTurn(CCObject* sender);
	void ActiveMyFightArea();
	void DeactiveMyFightArea();

	void OnSelectCardOK();
	void onClickBackground(CCObject* sender);
	cocos2d::extension::UILayer* GetLayer(){ return m_ui;}
	KActor& GetActor(){ return m_actor;}
private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;
	void InitTest();
	void RunTest(CCObject*);
	KActor m_actor;
	KSelectBeginCardPanel m_selectCardPanel;
	KResPanel m_resPanel;
	KIndicatePanel m_indicatePanel;
	KFightAreaPanel m_myFightAreaPanel;
	KGameResultPanel m_gameResultPanel;
};

#endif // __BattleFieldScene_SCENE_H__
