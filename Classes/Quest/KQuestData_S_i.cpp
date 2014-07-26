#include "KQuestData_S_i.h"
#include "CommonLogic/Quest/KQuestCommon.h"

KPlayerQuestData_S_i::KPlayerQuestData_S_i()
{
	m_CampQuestRFTime.clear();
}

KPlayerQuestData_S_i::~KPlayerQuestData_S_i()
{
}

void KPlayerQuestData_S_i::Finalize()
{
}

BOOL KPlayerQuestData_S_i::AddQuest(int qid)
{
	return TRUE;
}

BOOL KPlayerQuestData_S_i::QuestOk(int qid, TIME_T acceptTime)
{
	return TRUE;
}

BOOL KPlayerQuestData_S_i::CancelQuest(int qid)
{
	return TRUE;
}

BOOL KPlayerQuestData_S_i::HasActiveQuest(int qid) const
{
	int qidx = this->GetQuestIndex(qid);
	return qidx >= 0;
}

const KQuestNew* KPlayerQuestData_S_i::GetActiveQuest(int qid) const
{
	return NULL;
}

int KPlayerQuestData_S_i::GetActiveQuests(const KQuestNew* quests[], int maxNum) const
{
	int c = m_activeQuestArray.size();
	if (c > maxNum) c = maxNum;

	for (int i=0; i<c; i++)
	{
		quests[i] = m_activeQuestArray[i].m_pQuest;
	}

	return c;
}

int KPlayerQuestData_S_i::GetActiveQuestCount() const
{
	return m_activeQuestArray.size();
}


BOOL KPlayerQuestData_S_i::GetQuestHistory(int qid, int& val, time_t& t)
{
	KDBQuestHistoryDataUnit* pu = this->GetQuestHistoryUnit(qid);
	if(!pu || !pu->val) return FALSE;

	val = pu->val;
	t = KQuestCommon::GetUniTime(pu->time_h);

	return TRUE;
}

BOOL KPlayerQuestData_S_i::SetQuestHistory(int qid, int val, time_t t)
{
	return TRUE;
}

BOOL KPlayerQuestData_S_i::ClearHistory(int qid)
{
	return TRUE;
}

BOOL KPlayerQuestData_S_i::ClearAllHistory(void)
{
	return TRUE;
}

int KPlayerQuestData_S_i::GetAvailQuestIndex()
{
	return -1;
}

int KPlayerQuestData_S_i::GetQuestIndex(int qid, int* pArrPos) const
{
	return -1;
}

KDBQuestHistoryDataUnit* KPlayerQuestData_S_i::GetQuestHistoryUnit(int qid)
{
	KDBQuestHistoryDataUnit u = {static_cast<DWORD>(qid), 0, 0};
	int pos = m_questHistory.find(u);
	if(pos < 0) return NULL;

	KDBQuestHistoryDataUnit& hu = m_questHistory[pos];
	return &hu;
}

KDBQuestHistoryDataUnit* KPlayerQuestData_S_i::GetQuestHistoryUnit_force(int qid)
{
	return NULL;
}
