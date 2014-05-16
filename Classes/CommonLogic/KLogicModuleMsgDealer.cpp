#include "KLogicModuleMsgDealer.h"
#include "../Inc/KLogicMsg.h"
#include "../KMessageHub/KBattleMsgHub.h"
#include "../KMessageHub/KMsgHub.h"

KLogicModuleMsgDealer::KLogicModuleMsgDealer()
{
	
}

KLogicModuleMsgDealer::~KLogicModuleMsgDealer()
{

}

void KLogicModuleMsgDealer::RegisterAllMsg()
{
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_TURNBEGIN, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::TurnBegin);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_TURNEND, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::TurnEnd);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_INIT, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::BattleInit);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_DRAWCARD, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::DrawCard);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_DUELRESULT, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::CardDuelResult);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_CARDMOVE, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::CardMove);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_USERES, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::UseRes);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_HANDCARD_READY, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::HandCardReady);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_SELECTCARD_OK, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::SelectCardOK);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_ABILITYRESULT, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::AbilityResult);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_CARD2TOMB, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::Card2Tomb);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_DECKINIT, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::DeckInit);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_USE_SECRET, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::UseSecret);
	
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_DIRTY, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::BattleDirty);
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_OPDONE, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::BattleOpDone);
	
	RegisterMsg<KBattleMsgHub>(LOGIC_BATTLE_GAMEEND, &KMsgHub::getSingleton().GetBattleMsgHub(), &KBattleMsgHub::GameEnd);

	RegisterMsg<KWorldMsgHub>(LOGIC_WORLD_GEN_PLAYERCARD, &KMsgHub::getSingleton().GetWorldMsgHub(), &KWorldMsgHub::GenPlayerCard);
	RegisterMsg<KWorldMsgHub>(LOGIC_WORLD_USE_ITEM, &KMsgHub::getSingleton().GetWorldMsgHub(), &KWorldMsgHub::UseItem);
	
	
}


void KLogicModuleMsgDealer::Setup()
{
	m_MsgMap.init(LOGIC_BATTLE_COUNT - LOGIC_BATTLE_TURNBEGIN,LOGIC_BATTLE_TURNBEGIN);
	RegisterAllMsg();
}
	

