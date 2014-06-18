#ifndef _K_PLAYER_QUEST_MANAGER_H_
#define _K_PLAYER_QUEST_MANAGER_H_

#include <System/KPlatform.h>
#include <System/Collections/DynArray.h>
#include "./KQuestNew.h"
#include "CommonLogic/Quest/KQuestPersist.h"
#include "KDBDataStruct.h"
#include "../EventSystem/KEventInterface.h"

enum
{

	normal_quest_begin   = 1,		// 普通任务begin
	normal_quest_end     = 2,
	zhixian_quest_begin  = 2,		// 支线任务begin
	zhixian_quest_end	 = 3,
	daily_quest_begin    = 3,		// 日常任务begin
	prof_quest_begin	 = 3,		// 宗门任务begin
	prof_quest_end		 = 4,
	daily_quest_end      = 2001,
	activity_quest_begin = 2001,
	activity_quest_end   = 3001,
	invalid_quest_begin  = 99999,
};

namespace KWorldObjAbout
{
	class KPlayer;
	class KCharacter;
}

class KPlayerQuestManager : public IPersistObserver , public KEventAbout::IEventInterface
{
public:
	enum { max_trace_quest = 5 };

	class questCmp
	{
	public:
		typedef KQuestNew* ptr;
		int operator () (const ptr& a, const ptr& b) const
		{
			return a->m_qid - b->m_qid;
		}
	};
	typedef System::Collections::DynArray<KQuestNew*> PlayerQuestArray;
	typedef System::Collections::KDynSortedArray<KQuestNew*,questCmp> QuestSortArray;
	typedef System::Collections::DynArray<int> QuestIDArray;

	// 按设置成显示跟踪文字状态的时间排序
	class traceTickCmp
	{
	public:
		typedef KQuestNew* ptr;
		int operator () (const ptr& a, const ptr& b) const
		{
			DWORD t1 = a->GetSession(Session_TraceTick,0);
			DWORD t2 = b->GetSession(Session_TraceTick,0);
			if(t1 < t2)
			{
				return -1;
			}
			else if(t1 == t2)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	};
	typedef System::Collections::KDynSortedArray<KQuestNew*,traceTickCmp> TraceTickSortArray;

	class KHistoryCmp
	{
	public:
		BOOL operator () (const KDBQuestHistoryDataUnit& a, const KDBQuestHistoryDataUnit& b) const
		{
			return a.qid < b.qid;
		}
	};
	typedef System::Collections::DynSortedArray<KDBQuestHistoryDataUnit,KHistoryCmp> KQuestHistory;

public:
	KPlayerQuestManager();
	~KPlayerQuestManager();

public:
	void Reset();

public:
	void OnQuestAttr(int qid, int attrId, const LuaString& val);
	void OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val);


//event
public:
	virtual void handleEvent(int evt, const void* data, int len);
	void regAllEvent();
	virtual bool OnEventImp(KEventAbout::KEventID id, const KEventAbout::KEventData* pData, uint_r customData);
public:
	void on_loadQuest(KQuestNew* pQuest);
	void on_syncSession(KQuestNew* pQuest, DWORD key);
	void on_syncPersistAttr(KQuestNew* pQuest);
	void on_removeQuest(KQuestNew* pQuest);


public:
	KQuestNew* GetQuest(int qid);
	KQuestNew* GetAvailQuest(int qid);

	int GetQuests(int typeBeg, int typeEnd, KQuestNew** quests, int count);
	int GetAvailQuests(int typeBeg, int typeEnd, KQuestNew** quests, int count);
	int GetAvailQuestSize() { return m_availQuests.size();}

	KQuestNew* AddQuest(KQuestNew* pQuest);
	bool AddAvailQuest(KQuestNew* pQuest);

	bool RemoveQuest(int qid);
	bool RemoveAvailQuest(int qid);

	int RemoveQuests(int typeBeg, int typeEnd);
	int RemoveAvailQuests(int typeBeg, int typeEnd);

	void ClearAvailQuests();
	void ClearTmpQuests();

	// History Data
	void HistoryInit(int nCount, void * pData, int nLen);
	void HistorySync(int qid, int val, time_t lastDo);

	KDBQuestHistoryDataUnit* GetQuestHistoryUnit(int qid);
	KDBQuestHistoryDataUnit* GetQuestHistoryUnit_force(int qid);
	BOOL GetQuestHistory(int qid, int& val, time_t& t);
	BOOL SetQuestHistory(int qid, int val, time_t t);
	BOOL ClearHistory(int qid);
	BOOL ClearAllHistory(void);
	bool IsQuestEverDone(int qid);
	bool IsQuestEverDone(int qid, int dayOffset);

	KQuestNew* QueryNormalQuest();
	KQuestNew* RndQueryDailyQuest();
public:
	KQuestNew* m_dailyQuest;
	QuestSortArray m_quests;
	QuestSortArray m_availQuests;

	KQuestHistory m_questHistory;
	// 在KQuestPersist::goThrough时记住涉及哪些任务
	QuestSortArray m_tmpQuests;

	KWorldObjAbout::KPlayer* m_pPlayer;
};


typedef KPlayerQuestManager::PlayerQuestArray KPlayerQuestArray;
typedef KPlayerQuestManager::QuestSortArray KQuestSortArray;

#endif
