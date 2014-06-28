#include "KClientBattleCtrl.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "../Quest/KQuestNew.h"

KClientBattleCtrl* KClientBattleCtrl::getInstance(void)
{
	static KClientBattleCtrl sClientBattleCtrl;
	return &sClientBattleCtrl;
}


void KClientBattleCtrl::onTurnBegin(KBattleGuy* guy)
{
	m_CurPlayGuy = guy;
	m_CurOp.Empty();
}


