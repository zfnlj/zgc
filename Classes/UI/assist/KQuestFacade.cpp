#include "KQuestFacade.h"
#include "../../Quest/KPlayerQuestManager.h"
#include "../../KNet/KNetMsgFacade.h"
#include "../../WorldObject/KMainPlayer.h"
#include "../../WorldObject/KPlayer.h"
#include "GameRoot.h"
#include "../../KNet/KSocketFacade.h"
#include "KUIAssist.h"
#include "../MainMenuScene.h"
#include "../../VirtualService.h"
#include "../../Quest/KQuestManager.h"
#include "../BattleFieldScene.h"
#include "../../PlayerCard/KTowerAssist.h"

bool KQuestFacade::_startBattle()
{
	
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;

	KQuestNew* pQuest = playerQuestManager.QueryNormalQuest();
	if(!pQuest) return false;

	if(pQuest &&pQuest->GetQuestStatus()==KQ_PreStepOver){
		if(pQuest->IsSelectGift()){
			GameRoot::getSingleton().getBattleScene()->GameResult().onQuestPreOver(pQuest);
			return false;
		}else{
			VirtualService::getSingleton().SubmitQuest(pQuest->GetID());
			return _startBattle();
		}
		
	}

	if(!playerQuestManager.GetQuest(pQuest->GetID())){  // 任务不未接
		if(KClientBattleCtrl::getInstance()->IsServerSide()){
			KNetMsgFacade::onAcceptQuest(pQuest->GetID());
		}else{
			KSocketFacade::DoAcceptQuest(pQuest->GetID());
		}
	}

	KClientBattleCtrl::getInstance()->Play(pQuest,scene_battle);
	return true;
}

bool KQuestFacade::_startTower(int qId)
{
	KQuestNew* pTowerQuest = KQuestManager::GetInstance()->GetQuest(qId);
	int pos = KTowerAssist::_getPos(KMainPlayer::RealPlayer()->GetPlayerRecord());
	KQuestNew* pQuest = KQuestManager::GetInstance()->RndDailyQuest(pos);
	if(pQuest) pTowerQuest->m_battleField = pQuest->m_battleField;
	KClientBattleCtrl::getInstance()->Play(pTowerQuest,scene_tower);
	return true;
}


bool KQuestFacade::_startDaily(int qId)
{

	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;

	KQuestNew* pQuest = playerQuestManager.GetAdventureQuest();

	if(pQuest &&pQuest->GetQuestStatus()==KQ_PreStepOver){
		if(pQuest->IsSelectGift()){
			GameRoot::getSingleton().getBattleScene()->GameResult().onQuestPreOver(pQuest);
			return false;
		}else{
			VirtualService::getSingleton().SubmitQuest(pQuest->GetID());
			return _startDaily(qId);
		}

	}
	KNetMsgFacade::onAcceptQuest(qId);
	pQuest = playerQuestManager.GetQuest(qId);
	KClientBattleCtrl::getInstance()->Play(pQuest,scene_daily);
	return true;
}