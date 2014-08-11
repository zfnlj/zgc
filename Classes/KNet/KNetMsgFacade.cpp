#include "KNetMsgFacade.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "../GameLogic/KBattleCtrlBase.h"
#include "../GameRoot.h"
#include "KSocketDefine.h"
#include "../GameLogic/KGameDef.h"
#include "../StaticTable/KGameStatic.h"
#include "../Store/KStoreManager.h"
#include "../Item/KBagManager.h"
#include "../WorldObject/KMainPlayer.h"
#include "../UI/BattleFieldScene.h"
#include "../UI/MainMenuScene.h"
#include "../Quest/KQuestManager.h"
#include "../Quest/KPlayerQuestManager.h"
#include "../Quest/KQuestOperation.h"
#include "../StaticTable/StaticData.h"
#include "../WorldObject/KPlayer.h"
#include "../common/KUIMsgDef.h"

void KNetMsgFacade::onBattleInit()
{
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_INIT,0,0);
}

void KNetMsgFacade::onTurnBegin(const void* pData)
{
	KBattleGuy* guy = GameRoot::getSingleton().BattleCtrl()->GetCurGuy();
	KClientBattleCtrl::getInstance()->onTurnBegin(guy);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_TURNBEGIN,(unsigned long long)guy->GetFacade(),0);
}

void KNetMsgFacade::onSelectHandCardOK(UINT64 playerId)
{
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl()->GetGuy(playerId)->GetFacade();
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_SELECTCARD_OK,(unsigned long long)guy,0);
}

void KNetMsgFacade::onCardMove(int realId)
{
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_CARDMOVE,realId,0);
	
}

void KNetMsgFacade::onCardDuelResult(int atk,int def,int val1,int val2)
{
	strCardDuelResult result;
	result._atker = GameRoot::getSingleton().BattleCtrl()->GetCard(atk);
	result._defender = GameRoot::getSingleton().BattleCtrl()->GetCard(def);
	result._val1 = val1;
	result._val2 = val2;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DUELRESULT,(unsigned long long)&result,0);
}

void KNetMsgFacade::onUseAbilityResult(int src,int des,int abilityId,int val)
{
	strCardAbilityResult result;
	result._src = src;
	result.SetDestVal(des,val);
	result._pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(abilityId);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_ABILITYRESULT,(unsigned long long)&result,0);
}

void KNetMsgFacade::onBattleDrawCard(KCardInstList* list)
{
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DRAWCARD,(unsigned long long)list,0);
}

void KNetMsgFacade::onCard2Tomb(int realId)
{
	KCardInst* card  = KClientBattleCtrl::getInstance()->GetCard(realId);
	if(card){
		KClientBattleCtrl::getInstance()->onCard2Tomb(card);
	}
}

void KNetMsgFacade::onBattleSummonCard(int src,int des)
{
	/*strSummonCardResult result;
	result._src = GameRoot::getSingleton().BattleCtrl().GetCard(src);
	result._des = GameRoot::getSingleton().BattleCtrl().GetCard(des);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_SUMMONCARD,(unsigned long long)&result,0);*/
}

void KNetMsgFacade::onStoreAddProduct(const void* pData,int len)
{
	KStoreAbout::KStoreManager::Instance()->AddOneProduct((const BYTE*)pData,len);
}

void KNetMsgFacade::onBagOperation(const void* pData,int len)
{
	const SC_BagOperation* pBO = (const SC_BagOperation*)pData;
	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	pBagMgr->ItemOperator(pBO);
	GameRoot::getSingleton().getMainMenuScene()->onUpdateBag();
	if(GameRoot::getSingleton().BattleCtrl()->IsServerSide()){
		KMainPlayer::RealPlayer()->GetPlayerRecord()->updateMask(tb_player_record::_NORMALITEM);
	}
}

void KNetMsgFacade::onInitOneBag(const void* pData,int len)
{
	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	pBagMgr->InitOneBag(pData, len);
}

void KNetMsgFacade::onChangeMoney(const void* pData, int len)
{
	KMainPlayer::RealPlayer()->m_money.m_money = (const char*)pData;
	GameRoot::getSingleton().getMainMenuScene()->onUpdateMoney();
	if(GameRoot::getSingleton().BattleCtrl()->IsServerSide()){
		KMainPlayer::RealPlayer()->GetPlayerRecord()->updateMask(tb_player_record::_CRI);
	}
}

void KNetMsgFacade::syncAvailQuests(const void* pData, int len)
{
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;

	const SC_SyncAvailQuests* packet = (const SC_SyncAvailQuests*)pData;
	int count = len / sizeof(int);

	playerQuestManager.ClearAvailQuests();

	for(int i=0; i<count; i++)
	{
		int qid = packet->qids[i];
		KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qid);
		if(!pQuest)
		{
			Log(LOG_ERROR, "error: SC_SyncAvailQuests quest %d not found", qid);
			continue;
		}
		playerQuestManager.AddAvailQuest(pQuest);
	}

	KQuestChangeParam param;
	param.type = KQuestChangeParam::CAN_ACCEPT;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_WORLD_MAINPLAYER_QUEST_UPDATELIST, 0, (unsigned long)&param);
}

void KNetMsgFacade::onQuestFinished(int qId)
{
	KMainPlayer::RealPlayer()->GetResultBag()->Reset();
	KMainPlayer::RealPlayer()->GetQuestHolder().SubmitQuest(qId,0);
	/*KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	playerQuestManager.SetQuestHistory(qId,1,1);*/
	//GameRoot::getSingleton().getMainMenuScene()->onQuestFinished(qId);
	switch(qId){
	case 10004:
		STATIC_DATA_SET_KEYVAL("open_cardgroup");
		KDynamicWorld::getSingleton().onSystemMsg(OPEN_CARDGROUP);
		break;
	case 10008:
		STATIC_DATA_SET_KEYVAL("open_shop");
		KDynamicWorld::getSingleton().onSystemMsg(OPEN_SHOP);
		break;
	case 10011:
		STATIC_DATA_SET_KEYVAL("open_adventure");
		KDynamicWorld::getSingleton().onSystemMsg(OPEN_ADVENTURE);
		break;
	}
}

void KNetMsgFacade::onCancleQuest(int qId)
{
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = playerQuestManager.GetQuest(qId);
	if(!pQuest) return;
	KMainPlayer::RealPlayer()->CancelQuest(pQuest);
}

bool KNetMsgFacade::onAcceptQuest(int qId)
{
	return KMainPlayer::RealPlayer()->AcceptQuest(qId);
}