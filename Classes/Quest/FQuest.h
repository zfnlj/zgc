#ifndef _F_QUEST_H_
#define _F_QUEST_H_

#include "../Common/LogicWorldPrerequisites.h"
//#include "FPlayer.h"
#include "Inc/Lua/KLuaWrap.h"
#include "CommonLogic/Quest/KQuestAttrs.h"
#include "System/Collections/KString.h"

using namespace System::Collections;

class KQuestNew;
class FPlayer;

namespace KWorldObjAbout
{
	class KPlayer;
	class KCharacter;
}

class FQuest
	: public KFacadeObject<FQuest,KQuestNew>
{
friend class FMainPlayer;

public:
	KQuestNew* QuestNew();

public:
	explicit FQuest();
	~FQuest();

public:
	KWorldObjAbout::KPlayer* GetPlayer();

public:
	int GetID();
	int GetSession(const char* name, int defVal);
	int GetIntAttr(const char* name, int defVal);
	LuaString GetStringAttr(const char* name, const char* defVal);
	KQuestStatusCode GetStatus();
	bool IsTraceMode();

public:
	LuaString GetName(FPlayer* player);
	LongLuaString GetStatusDesc(FPlayer* player);
	LongLuaString GetTalkDesc(FPlayer* player);
	LongLuaString GetNoFinishDesc(FPlayer* player);
	LongLuaString GetFinishDesc(FPlayer* player);

	LongLuaString GetDesc(FPlayer* player);
	void GetLimitCondition(KDString<512>& cStr);
	void GetGiftDesc(KDString<512>& giftStr);	// LongLuaString太短
	void GetGiftDescWithExtraExp(KDString<512>& giftStr, int aExp, int aTimes);
	LuaString GetTraceText();
	LuaString GetNPCName(const char* type);	//任务npc名字
	LuaString GetTypeText(FPlayer* player);	// 任务类型名称
	void GetFigureAward(KDString<512>& giftStr);	// 得到任务数值奖励
	void GetItemAward(KDString<512>& giftStr);	// 得到任务物品奖励

public:

public:
	//BeginDefLuaClassNoCon(FQuest)
	//	DefMemberFunc(GetPlayer);
	//EndDef
};

#endif
