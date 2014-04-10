#pragma once

// 这里包括了所有无法定位到某个系统的记录集，目前有
// 每个技能或者每个道具一段时间内使用次数记录

#include <System/Collections/DynArray.h>
#include "KDBDataStruct.h"
#include "CommonLogic/Quest/KQuestSession.h"
#include "CommonLogic/Quest/KQuestPersist.h"
#include "KQuestNew.h"
#include "../common/KCommonObj.h"
struct KQuestPtr
{
	int m_qidx;
	KQuestNew* m_pQuest;
};
DECLARE_SIMPLE_TYPE(KQuestPtr);

class KPlayerQuestData_S_i : public KQuestPersist
{
public:
	class KQuestPtrCmp
	{
	public:
		BOOL operator () (const KQuestPtr& a, const KQuestPtr& b) const
		{
			return a.m_pQuest->m_qid < b.m_pQuest->m_qid;
		}
	};
	class KHistoryCmp
	{
	public:
		BOOL operator () (const KDBQuestHistoryDataUnit& a, const KDBQuestHistoryDataUnit& b) const
		{
			return a.qid < b.qid;
		}
	};
	typedef System::Collections::DynSortedArray<KQuestPtr,KQuestPtrCmp> KActiveQuestArray;
	typedef System::Collections::DynSortedArray<KDBQuestHistoryDataUnit,KHistoryCmp> KQuestHistory;
	typedef System::Collections::DynArray<int> KProfQuestList;

public:
	KPlayerQuestData_S_i();
	~KPlayerQuestData_S_i();

public:
	//BOOL Initialize(KTableCacheReader& tableCache, HANDLE hRec);
	void Finalize();

public:
	BOOL AddQuest(int qid);
	BOOL QuestOk(int qid, TIME_T acceptTime);
	BOOL CancelQuest(int qid);

	BOOL HasActiveQuest(int qid) const;
	const KQuestNew* GetActiveQuest(int qid) const;
	int GetActiveQuests(const KQuestNew* quests[], int maxNum) const;
	int GetActiveQuestCount() const;


	BOOL GetQuestHistory(int qid, int& val, time_t& t);
	BOOL SetQuestHistory(int qid, int val, time_t t);
	BOOL ClearHistory(int qid);
	BOOL ClearAllHistory(void);

protected:
	inline int qid_field(int qidx) const    { return qidx*2+1; }
	inline int qstate_field(int qidx) const { return qidx*2+2; }
	int GetAvailQuestIndex();
	int GetQuestIndex(int qid, int* pArrPos=NULL) const;
	KDBQuestHistoryDataUnit* GetQuestHistoryUnit(int qid);
	KDBQuestHistoryDataUnit* GetQuestHistoryUnit_force(int qid);

public:
	KQuestHistory m_questHistory;
	KActiveQuestArray m_activeQuestArray;
	KQuestSessions m_sessions[Max_Player_Active_Quest];


	typedef System::Collections::KMapByVector<int, DWORD> CAMPQUESTRFT;
	CAMPQUESTRFT m_CampQuestRFTime;	// 营地任务刷新时间

};
