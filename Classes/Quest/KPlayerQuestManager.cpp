#include "KPlayerQuestManager.h"
#include <System/Memory/KAutoPoolObjptr.h>
#include "../WorldObject/KPlayer.h"
#include "KClientQuestSession.h"
//#include <PacketGate/KGCPacket.h>
#include "CommonLogic/Quest/KQuestCommon.h"
#include "CommonLogic/Quest/KQuestArray.h"
#include "../Quest/KQuestManager.h"
#include "System/Misc/KDatetime.h"
#include "../GameLogic/KDynamicWorld.h"

KPlayerQuestManager::KPlayerQuestManager()
:m_pPlayer(NULL)
{

}

KPlayerQuestManager::~KPlayerQuestManager()
{

}

void KPlayerQuestManager::Reset()
{
	int c = m_quests.size();
	for(int i=c-1; i>=0; i--)
	{
		KQuestNew* pQuest = m_quests[i];
		pQuest->Reset();
		KQuestNew::Free(pQuest);
		m_quests.erase(i);
	}
	m_adventureQuest = NULL;
	m_questHistory.clear();	// 清空任务历史
	m_availQuests.clear();
	m_tmpQuests.clear();

	//m_pPlayer = NULL;
}

void KPlayerQuestManager::OnQuestAttr(int qid, int attrId, const LuaString& val)
{
	KQuestNew* pQuest = this->GetQuest(qid);
	if(!pQuest) return;

	pQuest->SetAttr(attrId, (char*)val.c_str());
	m_tmpQuests.insert_unique(pQuest);
}

void KPlayerQuestManager::OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val)
{
	KQuestNew* pQuest = this->GetQuest(qid);
	if(!pQuest) return;

	KTargetGroup* pGrp = pQuest->GetGroup(group);
	if(!pGrp) return;

	KQuestTarget* pTarget = pGrp->GetTarget(target);
	if(!pTarget) return;

	pTarget->SetAttr(name.c_str(), val.c_str());
	m_tmpQuests.insert_unique(pQuest);
}

void KPlayerQuestManager::on_loadQuest(KQuestNew* pQuest)
{
	// todo: ֪ͨUI
}

void KPlayerQuestManager::on_syncSession(KQuestNew* pQuest, DWORD key)
{
	static const DWORD keyTraceTick = ELFHash(Session_TraceTick);

	// todo: ֪ͨUI
}

void KPlayerQuestManager::on_syncPersistAttr(KQuestNew* pQuest)
{
	// todo: ֪ͨUI
}

void KPlayerQuestManager::on_removeQuest(KQuestNew* pQuest)
{
	// todo: ֪ͨUI
}

KQuestNew* KPlayerQuestManager::GetQuest(int qid)
{
	KAutoPoolObjPtr<KQuestNew> questPtr;
	questPtr->m_qid = qid;

	int pos = m_quests.find(questPtr);
	if(pos < 0) return NULL;

	return m_quests[pos];
}

KQuestNew* KPlayerQuestManager::GetAvailQuest(int qid)
{
	KAutoPoolObjPtr<KQuestNew> questPtr;
	questPtr->m_qid = qid;

	int pos = m_availQuests.find(questPtr);
	if(pos < 0) return NULL;

	return m_quests[pos];
}

int KPlayerQuestManager::GetQuests(int typeBeg, int typeEnd, KQuestNew** quests, int count)
{
	int num = 0;
	int c = m_quests.size();
	for(int i=0; i<c && num<count; i++)
	{
		KQuestNew* pQuest = m_quests[i];
		if(pQuest->m_type >= typeBeg && pQuest->m_type < typeEnd)
		{
			quests[num++] = pQuest;
		}
	}
	return num;
}

int KPlayerQuestManager::GetAvailQuests(int typeBeg, int typeEnd, KQuestNew** quests, int count)
{
	int num = 0;
	int c = m_availQuests.size();
	for(int i=0; i<c && num<count; i++)
	{
		KQuestNew* pQuest = m_availQuests[i];
		if(pQuest->m_type >= typeBeg && pQuest->m_type < typeEnd)
		{
			quests[num++] = pQuest;
		}
	}
	return num;
}

KQuestNew* KPlayerQuestManager::AddQuest(KQuestNew* pSrc)
{
	ASSERT(m_pPlayer);

	int pos = m_quests.find(pSrc);
	if(pos >= 0) return NULL;

	KQuestNew* pQuest = pSrc->Clone();
	m_quests.insert_unique(pQuest);

	KClientQuestSession* pSession = KClientQuestSession::Alloc();
	pSession->Reset();

	pQuest->m_pPlayer = m_pPlayer;
	pQuest->m_pSession = pSession;

	return pQuest;
}

bool KPlayerQuestManager::AddAvailQuest(KQuestNew* pQuest)
{
	int pos = m_quests.find(pQuest);
	if(pos >= 0) return false;
	m_availQuests.insert_unique(pQuest);
	return true;
}

bool KPlayerQuestManager::RemoveQuest(int qid)
{
	KAutoPoolObjPtr<KQuestNew> questPtr;
	questPtr->m_qid = qid;

	int pos = m_quests.find(questPtr);
	if(pos < 0) return false;

	KQuestNew* pQuest = m_quests[pos];
	m_quests.erase(pos);

	pQuest->Reset();
	KQuestNew::Free(pQuest);

	return true;
}

bool KPlayerQuestManager::RemoveAvailQuest(int qid)
{
	KAutoPoolObjPtr<KQuestNew> questPtr;
	questPtr->m_qid = qid;

	int pos = m_availQuests.find(questPtr);
	if(pos < 0) return false;

	KQuestNew* pQuest = m_availQuests[pos];
	m_availQuests.erase(pos);

	pQuest->Reset();
	KQuestNew::Free(pQuest);

	return true;
}

int KPlayerQuestManager::RemoveQuests(int typeBeg, int typeEnd)
{
	int num = 0;
	int c = m_quests.size();
	for(int i=c-1; i>=0; i--)
	{
		KQuestNew* pQuest = m_quests[i];
		if(pQuest->m_type >= typeBeg && pQuest->m_type < typeEnd)
		{
			num += 1;
			m_quests.erase(i);
			pQuest->Reset();
			KQuestNew::Free(pQuest);
		}
	}
	return num;
}

int KPlayerQuestManager::RemoveAvailQuests(int typeBeg, int typeEnd)
{
	int num = 0;
	int c = m_availQuests.size();
	for(int i=c-1; i>=0; i--)
	{
		KQuestNew* pQuest = m_availQuests[i];
		if(pQuest->m_type >= typeBeg && pQuest->m_type < typeEnd)
		{
			num += 1;
			m_availQuests.erase(i);
			pQuest->Reset();
			KQuestNew::Free(pQuest);
		}
	}
	return num;
}

void KPlayerQuestManager::ClearAvailQuests()
{
	m_availQuests.clear();
}

void KPlayerQuestManager::ClearTmpQuests()
{
	m_tmpQuests.clear();
}

void KPlayerQuestManager::HistoryInit(int nCount, void * pData, int nLen)
{
	KMsgInputStream is(pData, nLen);
	KDBQuestHistoryDataUnit dataUnit;
	for (int i=0; i<nCount; i++)
	{
		ASSERT_RETURN_VOID(is.ReadByteArray(&dataUnit, sizeof(KDBQuestHistoryDataUnit)));
		m_questHistory.insert_unique(dataUnit);
	}
}

void KPlayerQuestManager::HistorySync(int qid, int val, time_t lastDo)
{
	KDBQuestHistoryDataUnit* pu = this->GetQuestHistoryUnit_force(qid);
	pu->val = val;
	pu->time_h = lastDo;
}

KDBQuestHistoryDataUnit* KPlayerQuestManager::GetQuestHistoryUnit(int qid)
{
	KDBQuestHistoryDataUnit u = {qid, 0, 0};
	int pos = m_questHistory.find(u);
	if(pos < 0) return NULL;

	KDBQuestHistoryDataUnit& hu = m_questHistory[pos];
	return &hu;
}

KDBQuestHistoryDataUnit* KPlayerQuestManager::GetQuestHistoryUnit_force(int qid)
{
	KDBQuestHistoryDataUnit u = {qid, 0, 0};
	int pos = m_questHistory.find(u);
	if(pos < 0)
	{
		u.val = 0;
		u.time_h = 0;
		pos = m_questHistory.insert_unique(u); ASSERT(pos >= 0);
	}

	KDBQuestHistoryDataUnit& hu = m_questHistory[pos];
	return &hu;
}

BOOL KPlayerQuestManager::GetQuestHistory(int qid, int& val, time_t& t)
{
	KDBQuestHistoryDataUnit* pu = this->GetQuestHistoryUnit(qid);
	//if(!pu || !pu->val) return FALSE;
	if(!pu) return FALSE;
	val = pu->val;
	t = KQuestCommon::GetUniTime(pu->time_h);

	return TRUE;
}

BOOL KPlayerQuestManager::SetQuestHistory(int qid, int val, time_t t)
{
	ASSERT_I(val >= 0 && val <= 255);

	if(val < 0) val = 0;
	if(val > 255) val = 255;

	KDBQuestHistoryDataUnit* pu = this->GetQuestHistoryUnit_force(qid);

	pu->val = val;
	pu->time_h = _GetSystemTimeVal();
	return TRUE;
}

BOOL KPlayerQuestManager::ClearHistory(int qid)
{
	KDBQuestHistoryDataUnit u = {qid, 0, 0};
	int pos = m_questHistory.find(u);

	if(pos >= 0)
	{
		m_questHistory.erase(pos);
	}

	return TRUE;
}

BOOL KPlayerQuestManager::ClearAllHistory(void)
{
	m_questHistory.clear();
	return TRUE;
}

bool KPlayerQuestManager::IsQuestEverDone(int qid)
{
	int doTimes;
	time_t lastDo;

	if(!this->GetQuestHistory(qid, doTimes, lastDo))
	{
		KQuestNew* pQuest = this->GetQuest(qid);
		if(!pQuest) return false;
		int status = pQuest->GetSession(Session_Status, KQ_None);
		return status == KQ_QuestOver;
	}

	return doTimes > 0;
}

bool KPlayerQuestManager::IsQuestEverDone(int qid, int dayOffset)
{
	if(dayOffset == 0)
	{
		KQuestNew* pQuest = this->GetQuest(qid);
		if(pQuest && pQuest->GetSession(Session_Status, KQ_None) == KQ_QuestOver)
			return true;
	}

	int nTimes = 0;
	time_t ttLastTimes = 0;
	if (!this->GetQuestHistory(qid, nTimes, ttLastTimes))
		return false;

	if(nTimes < 1) return false;

	KDatetime now;
	int nowDays = now.ToLocalDays();

	KDatetime dt(ttLastTimes);
	int days = dt.ToLocalDays();

	return days == (nowDays+dayOffset);
}

void KPlayerQuestManager::handleEvent(int evt, const void* data, int len)
{

}

void KPlayerQuestManager::regAllEvent()
{
	KDynamicWorld::getSingleton().RegEvent(KEventAbout::enumWorldEvent_KillMonster,this,0,0);
}

bool KPlayerQuestManager::OnEventImp(KEventAbout::KEventID id, const KEventAbout::KEventData* pData, uint_r customData)
{
	int c = m_quests.size();
	for(int i=0; i<c; i++)
	{
		KQuestNew* pQuest = m_quests[i];
		pQuest->OnEventImp(id,pData,customData);
	}
	if(m_adventureQuest) m_adventureQuest->OnEventImp(id,pData,customData);
	return false;
}

KQuestNew* KPlayerQuestManager::RndQueryAdventureQuest(bool bDaily)
{
	if(bDaily) m_adventureQuest = RndQueryQuest(enum_daily_quest);
	if(m_adventureQuest) return m_adventureQuest;

	m_adventureQuest = RndQueryQuest(enum_zhixian_quest);
	return m_adventureQuest;
}

KQuestNew* KPlayerQuestManager::RndQueryQuest(enumQuestType questType)
{
	QuestSortArray dailyQuestArr;
	for(int i=0; i<m_quests.size(); i++)
	{
		KQuestNew* pQuest = m_quests[i];
		if(pQuest->m_type == questType) dailyQuestArr.insert_unique(pQuest);
	}
	int nRand = g_rnd.GetRandom(0,dailyQuestArr.size());
	KQuestNew* pQuest = dailyQuestArr[nRand];
	return pQuest;
}

KQuestNew* KPlayerQuestManager::QueryNormalQuest()
{
	for(int i=0; i<m_quests.size(); i++)
	{
		KQuestNew* pQuest = m_quests[i];
		if(pQuest->m_type == enum_normal_quest) return pQuest;
	}


	int num = m_availQuests.size();
	for(int i=0;i<num;i++)
	{
		KQuestNew* pQuest = m_availQuests[i];
		if(pQuest->m_type == enum_normal_quest){

			return pQuest;
		}
	}
	return NULL;
}
