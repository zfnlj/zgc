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
#include "KUIAssist.h"
#include "GameRoot.h"
#include "BattleFieldScene.h"
#include "../KNet/KUserData.h"
#include "../KNet/KSocketFacade.h"
#include "KJsonDictMgr.h"
#include "KClickCardMgr.h"

#include "../GameRecord/KGameRecordMgr.h"

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
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void BattleFieldScene::update(float dt)
{
	KGameRecordMgr::getSingleton().update(dt);
	m_actor.update(dt);
	m_myFightAreaPanel.update(dt);
	m_indicatePanel.Update(dt);
	KClickCardMgr::getSingleton().update(dt);
}

CCPoint BattleFieldScene::GetCenterPos()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint pt = CCDirector::sharedDirector()->getVisibleOrigin();
	pt.x += visibleSize.width/2;
	pt.y += visibleSize.height/2;
	return pt;
}

// on "init" you need to initialize your instance
bool BattleFieldScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;
	//if(KUserData::GetInstancePtr()->m_otpCode==0) //Î´Á¬Íø
	//	GameRoot::getSingleton().BattleCtrl().PlayWithAI();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BattleFieldScene::update),this,0.033f,false);
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	registerWithTouchDispatcher();

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(BattleFieldScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
  
    // add "BattleFieldScene" splash screen"

	this->addChild(GetUILayer(), 1);

	unsigned long long val = time(NULL);
	srand(val);//
    return true;
}


void BattleFieldScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


cocos2d::extension::UILayer* BattleFieldScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		UIWidget* widgetBattle = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/battle.json");
		m_ui->addWidget(widgetBattle);
		m_actor.init(widgetBattle);
		m_indicatePanel.init(m_ui);
		m_myFightAreaPanel.init(m_ui);
		m_gameResultPanel.init(m_ui);
		m_ui->getWidgetByName("turn_end")->addPushDownEvent(this, coco_pushselector(BattleFieldScene::DoEndTurn));
		m_ui->getWidgetByName("bk")->addPushDownEvent(this, coco_pushselector(BattleFieldScene::onClickBackground));
	}
	return m_ui;
}

void BattleFieldScene::onBattleInit()
{
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KUIAssist::_showCardSet("my_hero_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet("my_hand_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand));

	FBattleGuy* pOtherPlayer = GameRoot::getSingleton().BattleCtrl().GetOtherPlayer();
	KUIAssist::_showCardSet("your_hero_base",pOtherPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet("your_hand_base",pOtherPlayer->QueryCardSet(KCardInst::enum_slot_hand));

	if(GameRoot::getSingleton().BattleCtrl().IsSelectCard()){
		m_selectCardPanel.init(m_ui);
	}

	m_resPanel.init(m_ui);

	m_gameRecPanel.init(m_ui);

	InitTest();
}

void BattleFieldScene::InitTest()
{

	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KCardInst* pHero = (KCardInst*)pMainPlayer->GetHero();
	KCardActor* actor = (KCardActor*)pHero->getActor();
	actor->GetUI()->addPushDownEvent(this, coco_pushselector(BattleFieldScene::RunTest));
}

void BattleFieldScene::onClickBackground(CCObject* sender)
{
	GameRoot::getSingleton().BattleCtrl().GetCurOp().Empty();
	m_indicatePanel.OnSelectSrcCard(NULL);
	m_indicatePanel.Update(0.0f);
}

void BattleFieldScene::onSelectCardOK(FBattleGuy* guy)
{
	m_selectCardPanel.onSelectCardOK(guy);
}

void BattleFieldScene::onTurnBegin()
{

	UIButton* pBut = (UIButton*)m_ui->getWidgetByName("turn_end");
	if(GameRoot::getSingleton().BattleCtrl().IsMyTurn()){
		pBut->setTouchEnabled(true);
		pBut->setBright(true);
		m_actor.GetActionMgr().PlayAction("my_turn_begin");

	}else{
        pBut->setTouchEnabled(false);
		pBut->setBright(false);
		m_actor.GetActionMgr().PlayAction("your_turn_begin");
	}
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCurPlayer();
	m_resPanel.UpdateRes();
	KUIAssist::_updateCardListBuf(guy->QueryCardSet(KCardInst::enum_slot_fight));
}

void BattleFieldScene::onTurnEnd()
{

	KClickCardMgr::getSingleton().HideBigCard();
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCurPlayer();
	KUIAssist::_updateCardListBuf(guy->QueryCardSet(KCardInst::enum_slot_fight));
}

void BattleFieldScene::RunTest(CCObject* sender)
{
	/*FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	CCLabelBMFont* bmFont = CCLabelBMFont::create("-23","GUI/num_1.fnt");
	KCardInst* pHero = (KCardInst*)pMainPlayer->GetHero();
	KCardActor* actor = (KCardActor*)pHero->getActor();
	actor->GetUI()->addRenderer(bmFont,100);*/
	//m_gameResultPanel.ShowPanel(KGameResultPanel::enum_panel_task);
}

void BattleFieldScene::onHandCardReady()
{
	//FreshAllCard();
}

void BattleFieldScene::DoEndTurn(CCObject* sender)
{
	if(!KGameRecordMgr::getSingleton().IsClickButValidate(sender)) return;
	if(!GameRoot::getSingleton().BattleCtrl().IsMyTurn()) return;
	KGameRecordMgr::getSingleton().onMouseEvt(KRecordUIMouseData::evt_turn_end);

	if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		GameRoot::getSingleton().BattleCtrl().DoEndTurn();
	}else{
		KSocketFacade::DoEndTurn();
	}
	m_indicatePanel.OnSelectSrcCard(NULL);
}

void BattleFieldScene::ReGenerateAllCard()
{
	KClickCardMgr::getSingleton().onBattleRefresh();
	m_indicatePanel.onBattleRefresh();
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KUIAssist::_showCardSet("my_hero_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet("my_hand_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet("my_fight_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_fight));

	FBattleGuy* pPlayer = GameRoot::getSingleton().BattleCtrl().GetOtherPlayer();
	KUIAssist::_showCardSet("your_hero_base",pPlayer->QueryCardSet(KCardInst::enum_slot_hero));
	KUIAssist::_showCardSet("your_hand_base",pPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet("your_fight_base",pPlayer->QueryCardSet(KCardInst::enum_slot_fight));
	
}

void BattleFieldScene::FreshAllCard()
{
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KUIAssist::_showCardSet("my_hand_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet("my_fight_base",pMainPlayer->QueryCardSet(KCardInst::enum_slot_fight));
	KUIAssist::_removeCardSet(pMainPlayer->QueryCardSet(KCardInst::enum_slot_tomb));

	FBattleGuy* pPlayer = GameRoot::getSingleton().BattleCtrl().GetOtherPlayer();
	KUIAssist::_showCardSet("your_hand_base",pPlayer->QueryCardSet(KCardInst::enum_slot_hand));
	KUIAssist::_showCardSet("your_fight_base",pPlayer->QueryCardSet(KCardInst::enum_slot_fight));
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

		//int oldSlot = card->GetOldSlot();
		//if(oldSlot ==(int)KCardInst::enum_slot_hand){ //when hand card to fight, resort hand set.
		//	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(card);
		//	KUIAssist::_resortHandCardSet(guy);
		//}
	}
	
	m_indicatePanel.OnSelectCardOK();
}

void BattleFieldScene::onCardDuelResult(strCardDuelResult* result)
{
	KClickCardMgr::getSingleton().HideBigCard();
	KCardActor* atk = (KCardActor*)result->_atker->getActor();
	KCardActor* def = (KCardActor*)result->_defender->getActor();

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
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(pCard);
	KCardActor* actor = (KCardActor*)pCard->getActor();
	if(GameRoot::getSingleton().BattleCtrl().IsShowBack(pCard)){
		actor->UpdateUI();
		m_ui->addWidget(actor->GetUI());
	}
	KUIAssist::_resortHandCardSet(guy);
}

void BattleFieldScene::onDrawCard(KCardInstList* cardList,bool bInit)
{
	if(cardList->empty()) return;
	KCardInstList::iterator it = cardList->begin();
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(*it);
	if(GameRoot::getSingleton().BattleCtrl().GetBattleState()==KBattleCtrlBase::battle_init)
		return;
	UIImageView* base = NULL;
	if(guy==GameRoot::getSingleton().BattleCtrl().GetMainPlayer()){
		base = (UIImageView*)m_ui->getWidgetByName("my_slot_base");
	}else{
		base = (UIImageView*)m_ui->getWidgetByName("your_slot_base");
	}
	for(it=cardList->begin();it!=cardList->end();++it){
		KCardInst* card = *it;
		KCardActor* actor = KCardActor::create(card);
		if(bInit){
			actor->GetUI()->setPosition(base->getWorldPosition());
			actor->GetUI()->setScale(base->getScale());
		}
		m_ui->addWidget(actor->GetUI());
	}

	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	KCardInstList tmpLst;
	_copyCardSet(lst,&tmpLst,NULL,KCardInst::enum_slot_hand);
	KUIAssist::_moveCardSet(&tmpLst,"go_hand");

	//FreshAllCard();
}

void BattleFieldScene::ActiveMyFightArea()
{
	m_myFightAreaPanel.Show();
}

void BattleFieldScene::DeactiveMyFightArea()
{
	m_myFightAreaPanel.Hide();
}

void BattleFieldScene::onCardMove(KCardInst* pCard)
{
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(pCard);
	
	int oldSlot = pCard->GetOldSlot();
	KCardActor* actor = KCardActor::create(pCard); 
	if(actor->getBack()&& !GameRoot::getSingleton().BattleCtrl().IsShowBack(pCard)){
		actor->UpdateUI();
		m_ui->addWidget(actor->GetUI());
	}
	if(oldSlot ==(int)KCardInst::enum_slot_hand){ //when hand card to fight, resort hand set.
		KUIAssist::_resortHandCardSet(guy);
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
	GameRoot::getSingleton().BattleCtrl().GetCurOp().Empty();
	if(actor) GameRoot::getSingleton().BattleCtrl().GetCurOp()._src = actor->GetCard()->GetRealId();
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

void BattleFieldScene::onGameEnd()
{
	GameResult result;
	m_gameResultPanel.ShowPanel(result,KGameResultPanel::enum_panel_game);
}

void BattleFieldScene::onUseSecretCard(KCardInst* card)
{
	KCardActor* actor = (KCardActor*)card->getActor();
	actor->GetActionMgr().PlayAction("secret_use");

	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(card);
	KUIAssist::_resortHandCardSet(guy);
}

