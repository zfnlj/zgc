#include "FQuest.h"
#include "KQuestNew.h"
#include "../WorldObject/KPlayer.h"
#include "KQuestManager.h"

#define TO_KPLAYER(pfplayer) ((KWorldObjAbout::KPlayer*)pfplayer)

KQuestNew* FQuest::QuestNew()
{
	return this->GetFacadeImpl();
}

FQuest::FQuest()
{

}

FQuest::~FQuest()
{

}

KWorldObjAbout::KPlayer* FQuest::GetPlayer()
{
	KQuestNew* pQuest = this->QuestNew();
	if(!pQuest) return NULL;

	KWorldObjAbout::KPlayer* pPlayer = pQuest->GetPlayer();
	if(!pPlayer) return NULL;

	return pPlayer;
}

int FQuest::GetID()
{
	KQuestNew* pQuest = this->QuestNew();
	ASSERT(pQuest);
	return pQuest->m_qid;
}

int FQuest::GetSession(const char* name, int defVal)
{
	return this->GetFacadeImpl()->GetSession(name, defVal);
}

int FQuest::GetIntAttr(const char* name, int defVal)
{
	return this->GetFacadeImpl()->GetIntAttr(name, defVal);
}

LuaString FQuest::GetStringAttr(const char* name, const char* defVal)
{
	return LuaString(this->GetFacadeImpl()->GetStringAttr(name, defVal));
}

bool FQuest::IsTraceMode()
{
	return this->GetFacadeImpl()->IsTraceMode();
}

LuaString FQuest::GetName(FPlayer* player)
{
	char buf[1024];
	const char* pc = this->GetFacadeImpl()->GetName(TO_KPLAYER(player), buf, sizeof(buf));
	return LuaString(pc);
}

LongLuaString FQuest::GetStatusDesc(FPlayer* player)
{
	char buf[1024];
	const char* pc = this->GetFacadeImpl()->GetStatusDesc(TO_KPLAYER(player), buf, sizeof(buf));
	return LongLuaString(pc);
}

LongLuaString FQuest::GetTalkDesc(FPlayer* player)
{
	char buf[1024];
	const char* pc = this->GetFacadeImpl()->GetTalkDesc(TO_KPLAYER(player), buf, sizeof(buf));
	return LongLuaString(pc);
}

LongLuaString FQuest::GetFinishDesc(FPlayer* player)
{
	char buf[1024];
	const char* pc = this->GetFacadeImpl()->GetFinishDesc(TO_KPLAYER(player), buf, sizeof(buf));
	return LongLuaString(pc);
}

LongLuaString FQuest::GetDesc(FPlayer* player)
{
	char buf[1024];
	const char* pc = this->GetFacadeImpl()->GetDesc(TO_KPLAYER(player), buf, sizeof(buf));
	return LongLuaString(pc);
}

LuaString FQuest::GetTraceText()
{
	char buf[512];
	const char* text = this->GetFacadeImpl()->GetTraceText(buf, sizeof(buf));
	if(!text) return LuaString("empty trace text");

	return LuaString(buf);
}

KQuestStatusCode FQuest::GetStatus()
{
	return this->GetFacadeImpl()->GetQuestStatus();
}

void FQuest::GetGiftDesc(KDString<512>& giftStr)
{
	//this->GetFacadeImpl()->GetGiftDesc(KMainPlayer::Instance()->GetMainPlayer(), giftStr);
}

void FQuest::GetGiftDescWithExtraExp( KDString<512>& giftStr, int aExp, int aTimes )
{
	//this->GetFacadeImpl()->GetGiftDescWithExtraExp(KMainPlayer::Instance()->GetMainPlayer(), giftStr, aExp, aTimes);
}


/************************************************************************/
/* @param type NPCÀàÐÍ(npc/submitnpc)                                                                     */
/************************************************************************/
LuaString FQuest::GetNPCName( const char* type )
{
	//int npcid = this->GetIntAttr(type,0);
	////const char* name = KWorldObjAbout::KNpcBaseInfoManager::Instance()->GetNpcName(npcid);
	//const char* name = KQuestManager::GetInstance()->GetNpcPositionHref(npcid);
	//if(!name) return LuaString("npc not found");
	//return LuaString(name);
	return LuaString("npc not found");
}

LuaString FQuest::GetTypeText(FPlayer* player)
{
	char buf[32];
	const char* pc = this->GetFacadeImpl()->GetTypeText(TO_KPLAYER(player), buf, sizeof(buf));
	return LuaString(pc);
}

void FQuest::GetFigureAward( KDString<512>& giftStr )
{
	//KQuestNew* pQuest = this->QuestNew();
	//if (!pQuest)
	//{
	//	return;
	//}
	//this->GetFacadeImpl()->GetFigureGiftDesc(KMainPlayer::Instance()->GetMainPlayer(), giftStr);
}

void FQuest::GetItemAward( KDString<512>& giftStr )
{
	//this->GetFacadeImpl()->GetItemGiftDesc(KMainPlayer::Instance()->GetMainPlayer(), giftStr);
}

void FQuest::GetLimitCondition( KDString<512>& cStr )
{
	//this->GetFacadeImpl()->GetLimitCondition(KMainPlayer::Instance()->GetMainPlayer(), cStr);
}
