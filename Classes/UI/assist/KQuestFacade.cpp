#include "KQuestFacade.h"
#include "../Quest/KPlayerQuestManager.h"
#include "../KNet/KNetMsgFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "KUIAssist.h"
#include "../MainMenuScene.h"
#include "../VirtualService.h"
#include "../Quest/KQuestManager.h"
#include "../BattleFieldScene.h"

bool KQuestFacade::_startMainQuestBattle()
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
			return _startMainQuestBattle();
		}
		
	}

	if(!playerQuestManager.GetQuest(pQuest->GetID())){  // 任务不未接
		if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
			KNetMsgFacade::onAcceptQuest(pQuest->GetID());
		}else{
			KSocketFacade::DoAcceptQuest(pQuest->GetID());
		}
	}

	GameRoot::getSingleton().BattleCtrl().PlayQuestBattle(pQuest);
	return true;
}