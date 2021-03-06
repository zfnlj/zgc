#include "BattleFieldScene.h"
#include "ccMacros.h"
#include "../GameLogic/KDynamicWorld.h"
#include "GameRoot.h"
#include "../Facade/FBattleGuy.h"
#include "KCardActor.h"
#include "UILayer.h"
#include "cocos-ext.h"
#include "../Facade/FBattleGuy.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../GameLogic/KGameDef.h"
#include "assist/KUIAssist.h"
#include "GameRoot.h"
#include "BattleFieldScene.h"
#include "../KNet/KUserData.h"
#include "../KNet/KSocketFacade.h"
#include "assist/KJsonDictMgr.h"
#include "KClickCardMgr.h"
#include "../Quest/KQuestManager.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "assist/KQuestFacade.h"
#include "assist/KAudioAssist.h"
#include "KGameTipPanel.h"
#include "StageWaitScene.h"
USING_NS_CC;
using namespace cocos2d::extension;

CCScene* BattleFieldScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    BattleFieldScene *layer = BattleFieldScene::create();
	GameRoot::getSingleton().SetBattleScene(layer);
    // add layer as a child to scene
    scene->addChild(layer,0,1977);
	
    // return the scene
    return scene;
}

void BattleFieldScene::update(float dt)
{
	KAudioAssist::PlayBGM(KAudioAssist::audio_null,dt);
	KSceneLayerBase::update(dt);

	if(this==KUIAssist::_activeSceneLayer()){
		m_myFightAreaPanel.update(dt);
		m_indicatePanel.Update(dt);
		KClickCardMgr::getSingleton().update(dt);
		m_gameResultPanel.update(dt);
	}

}

// on "init" you need to initialize your instance
bool BattleFieldScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	//if(KUserData::GetInstancePtr()->m_otpCode==0) //δ����
	//	KClientBattleCtrl::getInstance()->PlayWithAI();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BattleFieldScene::update),this,0.033f,false);
    //////////////////////////////
    // 1. super init first
    if ( !KSceneLayerBase::init() )
    {
        return false;
    }
  
	CreateCloseBut();
    // add "BattleFieldScene" splash screen"

	if(!m_ui){
		m_ui = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/battle.json");
		addWidget(m_ui);
		m_actor.init(m_ui);
		m_indicatePanel.init(this);
		m_myFightAreaPanel.init(this);
		m_gameResultPanel.init(this);
		m_gameTipPanel.init(this);
		m_resPanel.init(this);
		m_gameRecPanel.init(this);
		this->getWidgetByName("turn_end")->addPushDownEvent(this, coco_pushselector(BattleFieldScene::DoEndTurn));
		this->getWidgetByName("bk")->addPushDownEvent(this, coco_pushselector(BattleFieldScene::onClickBackground));
	}
	m_actor.init(m_ui);

	if(KGameStaticMgr::getSingleton().GetLessonTip(StageWaitScene::m_qId))
	{
		m_gameTipPanel.ShowPanel();
	}
    return true;
}

void BattleFieldScene::onCloseCallback(CCObject* sender)
{
	KAudioAssist::PlayClickButSound();
	KClientBattleCtrl::getInstance()->ForceExit();
	KUIAssist::_switch2MainMenu();
}

void BattleFieldScene::onBattleInit()
{
	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand));

	FBattleGuy* pOtherPlayer = KClientBattleCtrl::getInstance()->GetOtherPlayer();
	KUIAssist::_showCardSet(pOtherPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	//KUIAssist::_moveCardSet(pOtherPlayer->QueryCardSet(KCardInst::enum_slot_hand),"go_hand");

	if(KClientBattleCtrl::getInstance()->IsSelectCard()){
		m_selectCardPanel.init(this);
	}else{
		KUIAssist::_showCardSet(pOtherPlayer->QueryCardSet(KCardInst::enum_slot_hand));
		//KUIAssist::_moveCardSet(pOtherPlayer->QueryCardSet(KCardInst::enum_slot_hand),"go_hand");
	}
	m_resPanel.UpdateRes();
	KAction::g_totalClassAction = 0;
	InitTest();
}

void BattleFieldScene::InitTest()
{

	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	KCardInst* pHero = (KCardInst*)pMainPlayer->GetHero();
	KCardActor* actor = (KCardActor*)pHero->getActor();

	FBattleGuy* pOtherPlayer = KClientBattleCtrl::getInstance()->GetOtherPlayer();
	KCardInst* pOtherHero = (KCardInst*)pOtherPlayer->GetHero();
	KCardActor* actor2 = (KCardActor*)pOtherHero->getActor();

	if(GameRoot::getSingleton().m_bDebug) {
		actor->GetUI()->addPushDownEvent(this, coco_pushselector(BattleFieldScene::RunTest));
		actor2->GetUI()->addPushDownEvent(this, coco_pushselector(BattleFieldScene::RunTest2));
	}
}

void BattleFieldScene::onClickBackground(CCObject* sender)
{
	KClientBattleCtrl::getInstance()->GetCurOp().Empty();
	m_indicatePanel.OnSelectSrcCard(NULL);
	m_indicatePanel.Update(0.0f);
}

void BattleFieldScene::onSelectCardOK(FBattleGuy* guy)
{
	m_selectCardPanel.onSelectCardOK(guy);
}

void BattleFieldScene::onTurnBegin()
{

	UIButton* pBut = (UIButton*)getWidgetByName("turn_end");
	if(KClientBattleCtrl::getInstance()->IsMyTurn()){
		pBut->setTouchEnabled(true);
		pBut->setBright(true);
		m_actor.GetActionMgr().PlayAction("my_turn_begin");

	}else{
        pBut->setTouchEnabled(false);
		pBut->setBright(false);
		m_actor.GetActionMgr().PlayAction("your_turn_begin");
	}
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCurPlayer();
	m_resPanel.UpdateRes();
	KUIAssist::_updateCardListBuf(guy->QueryCardSet(KCardInst::enum_slot_fight));
	
}

void BattleFieldScene::onTurnBeginDoubleDraw()
{
	if(KClientBattleCtrl::getInstance()->IsMyTurn()){
		m_actor.GetActionMgr().PlayAction("i_double_draw");
	}else{
		m_actor.GetActionMgr().PlayAction("u_double_draw");
	}
}

void BattleFieldScene::onTurnEnd()
{
	m_indicatePanel.OnSelectSrcCard(NULL);
	KClickCardMgr::getSingleton().HideBigCard();
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCurPlayer();
	KUIAssist::_updateCardListBuf(guy->QueryCardSet(KCardInst::enum_slot_fight));
}

void BattleFieldScene::RunTest(CCObject* sender)
{
	if(GameRoot::getSingleton().m_bDebug) {
		KClickCardMgr::getSingleton().HideBigCard();
		KClientBattleCtrl::getInstance()->ForceWin();
	}
	//m_gameResultPanel.ShowPanel();
	/*FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	CCLabelBMFont* bmFont = CCLabelBMFont::create("-23","GUI/num_1.fnt");
	KCardInst* pHero = (KCardInst*)pMainPlayer->GetHero();
	KCardActor* actor = (KCardActor*)pHero->getActor();
	actor->GetUI()->addRenderer(bmFont,100);*/
	//m_gameResultPanel.ShowPanel(KGameResultPanel::enum_panel_task);
}

void BattleFieldScene::RunTest2(CCObject* sender)
{
	if(GameRoot::getSingleton().m_bDebug) {
		KClickCardMgr::getSingleton().HideBigCard();
		KClientBattleCtrl::getInstance()->ForceLost();
	}
}

void BattleFieldScene::onHandCardReady()
{
	//FreshAllCard();
}

void BattleFieldScene::DoEndTurn(CCObject* sender)
{

	KAudioAssist::playSound("audio/ui/turn_end_click.wav");

	if(!KGameRecordMgr::getSingleton().IsClickButValidate(sender)) return;
	if(!KClientBattleCtrl::getInstance()->IsMyTurn()) return;
	
	KGameRecordMgr::getSingleton().onPlayStepOn();

	KUIAssist::_stopAdviceMsg();
	if(KClientBattleCtrl::getInstance()->IsServerSide()){
		KClientBattleCtrl::getInstance()->DoEndTurn();
	}else{
		KSocketFacade::DoEndTurn();
	}

}

void BattleFieldScene::ReGenerateAllCard()
{
	KClickCardMgr::getSingleton().onBattleRefresh();
	m_indicatePanel.onBattleRefresh();
	m_resPanel.UpdateRes();
	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_fight));

	FBattleGuy* pPlayer = KClientBattleCtrl::getInstance()->GetOtherPlayer();
	KUIAssist::_showCardSet(pPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet(pPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet(pPlayer->QueryCardSet(KCardInst::enum_slot_fight));
	
}

void BattleFieldScene::FreshAllCard()
{
	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_fight));
	KUIAssist::_removeCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_tomb));

	FBattleGuy* pPlayer = KClientBattleCtrl::getInstance()->GetOtherPlayer();
	KUIAssist::_showCardSet(pPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet(pPlayer->QueryCardSet(KCardInst::enum_slot_fight));
	KUIAssist::_removeCardSet(pPlayer->QueryCardSet(KCardInst::enum_slot_tomb));
}

void BattleFieldScene::onUseAbilityResult(strCardAbilityResult* result)
{
	KClickCardMgr::getSingleton().HideBigCard();
	K3DActionParam param1;
	param1.Copy(result);
	KUIAssist::_setActionParamSlot(&param1);
	if(result->_pAbility->ActionIsEmpty()) return;
	KCardActor* src = KUIAssist::_getCardActor(result->_actor);
	if(src){
		KCardInst* card = src->GetCard();
		if(src->getBack()){
			KUIAssist::_updateCard(card);
		}
		src->GetActionMgr().PlayAction(&param1);
	}
	
	m_indicatePanel.OnSelectCardOK();
}

void BattleFieldScene::onCardDuelResult(strCardDuelResult* result)
{
	KClickCardMgr::getSingleton().HideBigCard();
	KCardActor* atk = (KCardActor*)result->_atker->getActor();

	K3DActionParam param;
	param.init("duel");
	param.SetSrcVal(result->_atker->GetRealId(),result->_val1);
	param.SetDestVal(result->_defender->GetRealId(),result->_val2);
	KUIAssist::_setActionParamSlot(&param);
	atk->GetActionMgr().PlayAction(&param);

	m_indicatePanel.OnSelectCardOK();
}

void BattleFieldScene::onFighterBackHand(KCardInst* pCard)
{
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCardOwner(pCard);
	KCardActor* actor = (KCardActor*)pCard->getActor();
	if(KClientBattleCtrl::getInstance()->IsShowBack(pCard)){
		actor->UpdateUI();
		addWidget(actor->GetUI());
	}
	KUIAssist::_resortHandCardSet(guy);
}

void BattleFieldScene::onDrawCard(KCardInstList* cardList,bool bInit)
{
	if(cardList->empty()) return;
	KCardInstList::iterator it = cardList->begin();
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCardOwner(*it);
	if(KClientBattleCtrl::getInstance()->GetBattleState()==KBattleCtrlBase::battle_init||
		KClientBattleCtrl::getInstance()->GetBattleState()==KBattleCtrlBase::quest_battle_init)
		return;
	UIImageView* base = NULL;
	if(guy==KClientBattleCtrl::getInstance()->GetMainPlayer()){
		base = (UIImageView*)getWidgetByName("my_slot_base");
	}else{
		if(guy->IsSelectHandCard()) return;
		base = (UIImageView*)getWidgetByName("your_slot_base");
	}
	for(it=cardList->begin();it!=cardList->end();++it){
		KCardInst* card = *it;
		KCardActor* actor = KCardActor::create(card);
		if(bInit){
			actor->GetUI()->setPosition(base->getWorldPosition());
			actor->GetUI()->setScale(base->getScale());
		}
		addWidget(actor->GetUI());
	}

	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	KCardInstList tmpLst,tmpLst2;
	_copyCardSet(lst,&tmpLst,NULL,KCardInst::enum_slot_hand);
	int pos=0;
	for(KCardInstList::iterator it = tmpLst.begin();it!=tmpLst.end();++it,++pos){
		if(!(*it)->getActor()) continue;
		tmpLst2.push_back(*it);
	}
	KUIAssist::_playAction(&tmpLst2,"go_hand");
	it = tmpLst2.begin();
	if(it!=tmpLst2.end()){
		KCardActor* actor = (KCardActor*)(*it)->getActor();
		actor->GetActionMgr().PlayAction("delay_resortHand");
	}
	KAudioAssist::PlayDrawCardSound(cardList->size());
}

void BattleFieldScene::ActiveMyFightArea()
{
	m_myFightAreaPanel.Show();
}

void BattleFieldScene::DeactiveMyFightArea()
{
	m_myFightAreaPanel.Hide();
}

void BattleFieldScene::onSkillInterrupt(KCardInst* card)
{
	KCardActor* actor = KCardActor::create(card); 
	actor->GetActionMgr().PlayAction("skill_interrupt");
}

void BattleFieldScene::onCardMove(KCardInst* pCard)
{
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCardOwner(pCard);
	
	int oldSlot = pCard->GetOldSlot();
	KCardActor* actor = KCardActor::create(pCard); 
	if(actor->getBack()&& !KClientBattleCtrl::getInstance()->IsShowBack(pCard)){
		actor->UpdateUI(true);
		addWidget(actor->GetUI());
	}
	if(oldSlot ==(int)KCardInst::enum_slot_hand){ //when hand card to fight, resort hand set.

		if(pCard->GetST()->IsRare()&&
		pCard->GetSlot()==KCardInst::enum_slot_fight &&
		pCard->IsKindOf(KCardStatic::card_soldier)){
			actor->GetActionMgr().PlayAction("rare_enterfight");
			KUIAssist::_delayResortHandCardSet(guy);
		}else{
			KUIAssist::_resortHandCardSet(guy);
		}
	}
	
	actor->GetActionMgr().PlayAction("card_move");
	m_indicatePanel.OnSelectCardOK();
	KUIAssist::_soldierShow(pCard); // soldier enter fight movie.
}

void BattleFieldScene::onUseRes()
{
	m_resPanel.UpdateRes();
}

void BattleFieldScene::DoSelectSrcCard(KCardActor* actor)
{
	KClientBattleCtrl::getInstance()->GetCurOp().Empty();
	if(actor) KClientBattleCtrl::getInstance()->GetCurOp()._src = actor->GetCard()->GetRealId();
	m_indicatePanel.OnSelectSrcCard(actor);
	m_indicatePanel.Update(0.0f);

}

void BattleFieldScene::OnSelectCardOK()
{
	m_indicatePanel.OnSelectCardOK();
}

void BattleFieldScene::onSummonCard(strSummonCardResult* result)
{
	if(!result->_des) return;
	KCardActor* actor = KCardActor::create(result->_src);
	K3DActionParam param;
	param.init("summon");
	param.SetDestVal(result->_des->GetRealId(),0);
	actor->GetActionMgr().PlayAction(&param);
	OnSelectCardOK();
}


void BattleFieldScene::onUseSecretCard(KCardInst* card)
{
	KCardActor* actor = (KCardActor*)card->getActor();
	actor->GetActionMgr().PlayAction("secret_use");

	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCardOwner(card);
	KUIAssist::_resortHandCardSet(guy);
}

void BattleFieldScene::onEnterTransitionDidFinish()
{
	KSceneLayerBase::onEnterTransitionDidFinish();
}
