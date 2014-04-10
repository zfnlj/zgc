#include "FBattleGuy.h"
#include "../GameLogic/KBattleGuy.h"

int  FBattleGuy::QuerySlotCardNum()
{
	if(m_Impl)
		return m_Impl->QuerySlotCardNum();
	else
		return 0;
}

KCardInstList* FBattleGuy::QueryCardSet(int slot)
{
	if(m_Impl)
		return m_Impl->QueryCardSet(slot);
	else
		return NULL;
}

bool FBattleGuy::IsFirstHand()
{
	if(m_Impl)
		return m_Impl->IsFirstHand();
	else
		return false;
}

bool FBattleGuy::IsEmptyFightSlot(int pos)
{
	if(m_Impl)
		return m_Impl->GetDeck().IsEmptyFightSlot(pos);
	else 
		return false;
}

KCardInst* FBattleGuy::GetHero()
{
	if(m_Impl)
		return m_Impl->GetDeck().GetHero();
	else
		return NULL;
}

KCardInst* FBattleGuy::GetCard(int id)
{
	if(m_Impl)
		return m_Impl->GetDeck().GetCard(id);
	else
		return NULL;
}

void FBattleGuy::Clear()
{
	if(m_Impl) m_Impl->Clear();
}

void FBattleGuy::QueryResInfo(char* buf)
{
	if(m_Impl) m_Impl->QueryResInfo(buf);
}

bool FBattleGuy::IsSelectHandCard()
{
	if(m_Impl)
		return m_Impl->IsSelectHandCard();
	else
		return false;
}

void FBattleGuy::QueryActiveHandCards(KCardInstList* arr)
{
	if(m_Impl) m_Impl->QueryActiveHandCards(arr);
}

void FBattleGuy::QueryActiveDefendCards(KCardInstList* arr)
{
	if(m_Impl) m_Impl->QueryActiveDefendCards(arr);
}

void FBattleGuy::QueryActiveFightCards(KCardInstList* arr)
{
	if(m_Impl) m_Impl->QueryActiveFightCards(arr);
}
