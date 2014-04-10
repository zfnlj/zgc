#include "KBattleFieldStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>


bool KBattleFieldStatic::init()
{
	m_Id = m_myDeck = m_yourDeck = m_monster = 0;
	m_yourAI = npc_ai_null;
	return true;
}

KBattleFieldStatic* KBattleFieldStatic::create()
{
	KBattleFieldStatic* pBattle = new KBattleFieldStatic;
	pBattle->init();
	return pBattle;
}

void KBattleFieldStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	fileReader->GetInteger("MyDeck", 0, (int*)&m_myDeck);
	fileReader->GetInteger("YourDeck", 0, (int*)&m_yourDeck);
	fileReader->GetInteger("MyFirst", 0, (int*)&m_myFirst);
	fileReader->GetInteger("SelectCard", 0, (int*)&m_selectCard);
	fileReader->GetInteger("Monster", 0, (int*)&m_monster);
	int aiVal;
	fileReader->GetInteger("YourAI", 0, (int*)&aiVal);
	m_yourAI = (KBattleFieldStatic::enum_npc_ai)aiVal;
}
