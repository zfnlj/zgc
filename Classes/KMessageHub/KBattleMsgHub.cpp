#include "KBattleMsgHub.h"

#ifdef _USE_COCOS2DX
#include "../UI/BattleFieldScene.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#else
#include "../../Scene/KWorld.h"
#include "../../Facade/KWorldFacade.h"
#endif

#include "../Facade/FBattleGuy.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KGameDef.h"


KBattleMsgHub::KBattleMsgHub()
{
	
}

KBattleMsgHub::~KBattleMsgHub()
{

}

void KBattleMsgHub::BattleInit(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onBattleInit();
#else
	KWorldFacade::onBattleInit((KWorld*)Param2);
#endif
}

void KBattleMsgHub::TurnBegin(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onTurnBegin();
#else
	KWorldFacade::onTurnBegin((KBattleGuy*)Param1,(KWorld*)Param2);
#endif
}

void KBattleMsgHub::TurnEnd(unsigned long long Param1, unsigned long long Param2)
{
}

void KBattleMsgHub::DrawCard(unsigned long long Param1, unsigned long long Param2)
{
	KCardInstList* list =(KCardInstList*)Param1;
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onDrawCard(list);
#else
	KWorldFacade::onDrawCard(list,(KWorld*)Param2);
#endif
}

void KBattleMsgHub::AbilityResult(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onUseAbilityResult((strCardAbilityResult*)Param1);
#else
	KWorldFacade::onUseAbilityResult((KWorld*)Param2,(strCardAbilityResult*)Param1);
#endif
}

void KBattleMsgHub::CardDuelResult(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onCardDuelResult((strCardDuelResult*)Param1);
#else
	KWorldFacade::onCardDuelResult((KWorld*)Param2,(strCardDuelResult*)Param1);
#endif
}

void KBattleMsgHub::UseRes(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onUseRes();
#else
	KWorldFacade::onWorldDirty((KWorld*)Param2);
#endif
}

void KBattleMsgHub::CardMove(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	KCardInst* pCard = GameRoot::getSingleton().BattleCtrl().GetCard(Param1);
	GameRoot::getSingleton().getBattleScene()->onCardMove(pCard);
#else
	KWorldFacade::onWorldDirty((KWorld*)Param2);
	KWorldFacade::onCardMove((int)Param1,(KWorld*)Param2);
#endif
}

void KBattleMsgHub::Secret2Tomb(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onSecret2Tomb((KCardInst*)Param1);
#else
	
#endif
}

void KBattleMsgHub::Card2Tomb(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
#else
	KWorldFacade::onCard2Tomb((int)Param1,(KWorld*)Param2);
#endif
}

void KBattleMsgHub::UpdateGuyInfo(unsigned long long Param1, unsigned long long Param2)
{

}

void KBattleMsgHub::HandCardReady(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onHandCardReady();
#else
	
#endif
}

void KBattleMsgHub::SelectCardOK(unsigned long long Param1, unsigned long long Param2)
{
	FBattleGuy* pGuy = (FBattleGuy*)Param1;
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onSelectCardOK(pGuy);
#else
	KWorldFacade::onSelectHandCardOK(pGuy->GetImp(),(KWorld*)Param2);
#endif
}

void KBattleMsgHub::BattleDirty(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX

#else
	KWorldFacade::onBattleDirty((KWorld*)Param2);
#endif
}

void KBattleMsgHub::BattleOpDone(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	if(!GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		KSocketFacade::DoBattleOpDone((KBattleCtrlBase::BattleOp*)Param1);
	}
#else

#endif
}

void KBattleMsgHub::SummonCard(unsigned long long Param1, unsigned long long Param2)
{
	strSummonCardResult* result = (strSummonCardResult*)Param1;
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onSummonCard(result);
#else
	KWorldFacade::onSummonCard(result,(KWorld*)Param2);
#endif
}

void KBattleMsgHub::DeckInit(unsigned long long Param1, unsigned long long Param2)
{
	FBattleGuy* pGuy = (FBattleGuy*)Param1;
#ifdef _USE_COCOS2DX
#else
	KWorldFacade::onDeckInit(pGuy->GetImp(),(KWorld*)Param2);
#endif
}

void KBattleMsgHub::GameEnd(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getBattleScene()->onGameEnd();
#else
	KWorldFacade::onGameEnd((KBattleGuy*)Param1,(KWorld*)Param2);
#endif
}
