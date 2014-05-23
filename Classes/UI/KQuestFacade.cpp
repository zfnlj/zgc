#include "KQuestFacade.h"
#include "../Quest/KPlayerQuestManager.h"
#include "../KNet/KNetMsgFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"

bool KQuestFacade::_startMainQuestBattle()
{
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = playerQuestManager.QueryNormalQuest();
	if(!pQuest) return false;
	if(!playerQuestManager.GetQuest(pQuest->GetID())){  // ����δ��
		if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
			KNetMsgFacade::onAcceptQuest(pQuest->GetID());
		}else{
			KSocketFacade::DoAcceptQuest(pQuest->GetID());
		}
	}
	GameRoot::getSingleton().BattleCtrl().PlayQuestBattle(pQuest);
	return true;
}