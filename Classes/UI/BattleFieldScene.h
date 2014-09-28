#ifndef __BATTLEFIELDSCENE__H__
#define __BATTLEFIELDSCENE__H__

#include "cocos2d.h"
#include "KSceneLayerBase.h"
#include "KSelectBeginCardPanel.h"
#include "KResPanel.h"
#include "KIndicatePanel.h"
#include "KFightAreaPanel.h"
#include "KNotifyPanel.h"
#include "KActor.h"
#include "KGenCardPanel.h"
#include "KGameResultPanel.h"
#include "KGameRecordPanel.h"

USING_NS_CC;
class KCardInst;
struct strCardDuelResult;
struct strCardAbilityResult;
struct strSummonCardResult;
struct strReplaceCardResult;
class KCardInst;
class FBattleGuy;

class BattleFieldScene :public KSceneLayerBase
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
	virtual void onCloseCallback();
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(BattleFieldScene);


	KIndicatePanel& IndicatePanel(){ return m_indicatePanel;}
	void onBattleInit();
	void onHandCardReady();
	void FreshAllCard();
	void ReGenerateAllCard();
	void onCardDuelResult(strCardDuelResult* result);
	void onSummonCard(strSummonCardResult* result);
	void onUseAbilityResult(strCardAbilityResult* result);
	void onSelectCardOK(FBattleGuy* guy);
	void onTurnBegin();
	void onTurnEnd();
	void onDrawCard(KCardInstList* cardList,bool bInit=true);
	void onCardMove(KCardInst* pCard);
	void onFighterBackHand(KCardInst* card);
	void onUseSecretCard(KCardInst* card);
	void onUseRes();
	void onSkillInterrupt(KCardInst*);
	void DoEndTurn(CCObject* sender);
	void ActiveMyFightArea();
	void DeactiveMyFightArea();
	void RunTest2(CCObject* sender);

	void OnSelectCardOK();
	void onClickBackground(CCObject* sender);
	cocos2d::extension::UILayer* GetLayer(){ return this;}

	void DoSelectSrcCard(KCardActor* actor);
	KGameResultPanel& GameResult(){ return m_gameResultPanel;}

	virtual void onEnterTransitionDidFinish();
private:
	CCLabelTTF* m_InfoLabel;
	void InitTest();
	void RunTest(CCObject*);
	KSelectBeginCardPanel m_selectCardPanel;
	KResPanel m_resPanel;
	KIndicatePanel m_indicatePanel;
	KFightAreaPanel m_myFightAreaPanel;
	KGameResultPanel m_gameResultPanel;

};

#endif // __BattleFieldScene_SCENE_H__
