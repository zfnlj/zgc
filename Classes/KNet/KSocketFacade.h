#pragma once
#include "../GameLogic/KCardInst.h"
#include "../GameLogic/KBattleCtrlBase.h"

struct BattleOp;
class KSocketFacade
{
public:
	static void DoSelectBeginCard(KCardInstList* lst);
	static void DoBattleOpDone(KBattleCtrlBase::BattleOp* p);
	static void DoEndTurn();
	static void DoAcceptQuest(int id);
	static void DoCancelQuest(int id){}
	static void DoSubmitQuest(int id);
};
