#include "KQuestOperation.h"
#include "KQuestManager.h"
//#include "KQuestNew.h"
//#include "KServerQuestSession.h"
//#include "QuestLua.h"
#include "../../Inc/PacketGate/c_game_packet.h"
#include "../WorldObject/KMainPlayer.h"
#include "../common/KPlayerRecordAssist.h"
#include "System/Misc/KDatetime.h"
#include "../common/KUIMsgDef.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../PlayerCard/KTowerAssist.h"

using namespace System::Collections;
//using namespace KQuestLuaAbout;

KQuestHolderBase::KQuestHolderBase() : m_pPlayer(NULL)
{

}

KQuestHolderBase::~KQuestHolderBase()
{

}

void KQuestHolderBase::Breathe(int nInterval)
{
	int questCount = KMainPlayer::RealPlayer()->m_questManager.m_quests.size();
	for(int i=questCount-1; i>=0; i--)
	{
		KQuestNew* pQuest = KMainPlayer::RealPlayer()->m_questManager.m_quests[i];
		pQuest->Breathe(nInterval);
	}
}

KPlayerQuestManager::QuestSortArray& KQuestHolderBase::_GetPlayerQuestArray()
{
	return KMainPlayer::RealPlayer()->m_questManager.m_quests;
}

void KQuestHolderBase::_SetAdventureQuest(KQuestNew* quest)
{
	KMainPlayer::RealPlayer()->m_questManager.SetAdventureQuest(quest);
}

KPlayerQuestManager::QuestSortArray& KQuestHolderBase::_GetPlayerActiveQuestArray()
{
	return KMainPlayer::RealPlayer()->m_questManager.m_availQuests;
}


bool KQuestHolderBase::AcceptQuest(int qid)
{
	KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qid);
	if(!pQuest) return false;
	if(KMainPlayer::RealPlayer()->m_questManager.GetQuest(qid)) return false;

	if(!pQuest->CanDoThis(KMainPlayer::RealPlayer())) return false;


	KQuestNew* pClone = pQuest->Clone();
	pClone->SetQuestStatus(KQ_QuestRuning);
	if(!this->_AcceptQuestForce(pClone))
	{
		pClone->Reset();
		KQuestNew::Free(pClone);
		return false;
	}

	// Send Accept Quest Log To DB By Calvin
	//pQuestManager->SaveQuestLog(qid, pQuest->m_type, m_pPlayer, E_LogDB_QuestOperator_Accept);

	return true;
}

bool KQuestHolderBase::AcceptQuestForce(int qid)
{
	KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qid);
	if(!pQuest) return false;
	if(KMainPlayer::RealPlayer()->m_questManager.GetQuest(qid)) return false;

	KQuestNew* pClone = pQuest->Clone();
	if(!this->_AcceptQuestForce(pClone))
	{
		pClone->Reset();
		KQuestNew::Free(pClone);
		return false;
	}
	return true;
}

bool KQuestHolderBase::_AcceptQuestForce(KQuestNew* pQuest)
{
	//宗门任务接取前处理

	pQuest->m_pPlayer = m_pPlayer;
	if(pQuest->m_type==enum_normal_quest){
		if(!KPlayerRecordAssist::AddQuest(KMainPlayer::RealPlayer()->GetQuestRecord(),pQuest))
			return false;
		ASSERT(_GetPlayerQuestArray().insert_unique(pQuest) >= 0);
	}else{
		ASSERT(_GetPlayerQuestArray().insert_unique(pQuest) >= 0);
	}

	//KServerQuestSession* pSession = KServerQuestSession::Alloc();
	//pSession->Initialize(m_pPlayer, pQuest->m_qid);
	//pQuest->m_pSession = pSession;

	//this->SyncQuest(pQuest, false);

	//pQuest->OnAccept();

	// 这个时候再OnInit，因为任务的属性可能会在OnAccept中修改
	//pQuest->OnInit();


	// 刷新一次可接任务列表
	//KQuestManager::GetInstance()->SyncAvailQuests(m_pPlayer);

	return true;
}

void KQuestHolderBase::OnQuestAttr(int qid, int attrId, const LuaString& val)
{
	KQuestNew* pQuest = this->GetActiveQuest(qid);
	if(!pQuest) return;
	pQuest->SetAttr(attrId, (char*)val.c_str());
}

void KQuestHolderBase::OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val)
{
	KQuestNew* pQuest = this->GetActiveQuest(qid);
	if(!pQuest) return;

	KTargetGroup* pGroup = pQuest->GetGroup(group);
	if(!pGroup) return;

	KQuestTarget* pTarget = pGroup->GetTarget(target);
	if(!pTarget) return;

	pTarget->SetAttr(name.c_str(), (char*)val.c_str());
}

KQuestNew* KQuestHolderBase::GetActiveQuest(int qid)
{
	KQuestNew quest; quest.m_qid = qid;
	int pos = _GetPlayerQuestArray().find(&quest);
	if(pos < 0) return NULL;
	return _GetPlayerQuestArray()[pos];
}

bool KQuestHolderBase::IsQuestEverDone(int qid)
{
	int doTimes;
	time_t lastDo;

	if(!this->GetQuestHistory(qid, doTimes, lastDo))
	{
		KQuestNew* pQuest = this->GetActiveQuest(qid);
		if(!pQuest) return false;
		int status = pQuest->GetSession(Session_Status, KQ_None);
		return status == KQ_QuestOver;
	}

	return doTimes > 0;
}

bool KQuestHolderBase::IsQuestEverDone(int qid, int dayOffset)
{
	if(dayOffset == 0)
	{
		KQuestNew* pQuest = this->GetActiveQuest(qid);
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

bool KQuestHolderBase::QuestOk(KQuestNew* pQuest)
{
	//TIME_T acceptTime = (TIME_T)pQuest->GetSession(Session_AcceptTime, 0);
	//if(!acceptTime)
	//{
	//	Log(LOG_FATAL, "fatal: pid:%llu quest:%d has no accept time", m_pPlayer->m_id, pQuest->m_qid);
	//	acceptTime = time(NULL);
	//}

	pQuest->OnFinish();
	//pQuest->OnUninit();

	int qid = pQuest->m_qid;

	if(pQuest->m_type == enum_daily_quest){
		bool bLevUp=false;
		bool hasSlot = KPlayerRecordAssist::UseDailyAwardSlot(KMainPlayer::RealPlayer()->GetQuestRecord());
		if(hasSlot)KPlayerRecordAssist::DailyStageWin(KMainPlayer::RealPlayer()->GetPlayerRecord(),pQuest->m_hardDegree,bLevUp);
		m_pPlayer->m_questManager.onDailyOk();
		if(bLevUp){
			KDynamicWorld::getSingleton().onSystemMsg(NEW_DAILY_STAGE);
		}else if(hasSlot){
			bool bActive = KTowerAssist::_active(KMainPlayer::RealPlayer()->GetPlayerRecord(),pQuest->m_hardDegree);
			if(bActive) KDynamicWorld::getSingleton().onSystemMsg(ACTIVE_TOWER);
		}
	}else if(pQuest->m_type == enum_normal_quest){
		KPlayerRecordAssist::QuestOk(KMainPlayer::RealPlayer()->GetQuestRecord(),pQuest->m_qid);
		m_pPlayer->m_questManager.SetQuestHistory(qid,1,1);
	}
	


	

	// sync questhistory to client

	//if(!this->GetQuestHistory(qid, doTimes, lastDo)) 
	//{
	//	// TODO : questhistory更新没成功?
	//	return false;
	//}
	//SC_QuestHistorySync packet_h;
	//packet_h.dataUnit.qid = qid;
	//packet_h.dataUnit.val = doTimes;
	//packet_h.dataUnit.time_h = lastDo;
	//m_pPlayer->syncData(SC_QuestHistorySync::s_nCode, &packet_h, sizeof(packet_h), eSyncMyself);
	
	int pos = _GetPlayerQuestArray().find(pQuest);
	if(pos >= 0)
	{
		_GetPlayerQuestArray().erase(pos);
	}

	pQuest->Reset();
	KQuestNew::Free(pQuest);


	return true;
}

bool KQuestHolderBase::CancelQuest(KQuestNew* pQuest)
{
	if(!pQuest->IsAbandonAble()) return false;
	KPlayerRecordAssist::CancelQuest(KMainPlayer::RealPlayer()->GetQuestRecord(),pQuest->m_qid);
	int pos = _GetPlayerQuestArray().find(pQuest);
	if(pos >= 0)
	{
		_GetPlayerQuestArray().erase(pos);
	}

	// sync to client
	//SC_RemoveQuest packet;
	//packet.qid = pQuest->m_qid;
	//m_pPlayer->syncData(SC_RemoveQuest::s_nCode, &packet, sizeof(packet), eSyncMyself);

	pQuest->Reset();
	KQuestNew::Free(pQuest);

	// 更新下可接任务列表
	//KQuestManager::GetInstancePtr()->SyncAvailQuests(m_pPlayer);

	return true;
}

////////////////////////////////////玩家主接口//////////////////////////////////////

//加任务
KQuestNew* KQuestHolderBase::AddQuest(int qid)
{

	if(KMainPlayer::RealPlayer()->GetActiveQuest(qid)) return NULL;

	KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qid);
	if(!pQuest)
	{
		KString<512> str("quest does not exists");
		//m_pPlayer->PushStringMessage(enumECG_MessageBox, str.c_str(), str.size());
		return NULL;
	}

	KQuestNew* pQuestClone = pQuest->Clone();

	if(!_AcceptQuestForce(pQuestClone))
	{
		pQuestClone->Reset();
		KQuestNew::Free(pQuestClone);
		return NULL;
	}

	// Send Accept Quest Log To DB By Calvin
	//pQuestManager->SaveQuestLog(qid, pQuest->m_type, m_pPlayer, E_LogDB_QuestOperator_Accept);
	return pQuestClone;
}

//删除任务
bool KQuestHolderBase::RemoveQuest(int qid)
{
	KQuestNew* pQuest = this->GetActiveQuest(qid);
	if(!pQuest) return false;
	int status = pQuest->GetSession(Session_Status, KQ_None);
	switch(status)
	{
	case KQ_None:			// 未接受
		//ASSERT(FALSE);
		//return FALSE;
		return this->CancelQuest(pQuest);
	case KQ_PreStepOver:	// 任务完成没有交付
		return this->CancelQuest(pQuest);
	case KQ_QuestOver:		// 任务完成已经交付
		//ASSERT(FALSE);
		//return FALSE;
		return this->QuestOk(pQuest);
	case KQ_QuestRuning:	// 已接受，进行中
		return this->CancelQuest(pQuest);
	//case KQ_QuestFailed:	// 已经失败，需要放弃重新接受
	//	return this->QuestFail(pQuest);
	default:
		return false;
	}
}


//是否存在该任务
bool KQuestHolderBase::HasQuest(DWORD qid)
{
	return KMainPlayer::RealPlayer()->m_questManager.GetQuest(qid) != NULL;
}

bool KQuestHolderBase::GetQuestHistory(int qid, int& times, time_t& lastDo)
{
	return KMainPlayer::RealPlayer()->m_questManager.GetQuestHistory(qid, times, lastDo) ? true : false;
}


DWORD KQuestHolderBase::GetQuestViewIconStrID( DWORD qState, DWORD qType )
{
	switch (qState)
	{
	case KQ_None:
		{
			if(qType == enum_normal_quest)
			{
				return 1412011;
			}
			else if(qType == enum_zhixian_quest)
			{
				return 1412012;
			}
			else
				return 1412013;
		}
		break;
	case KQ_PreStepOver:
		{
			if(qType == enum_normal_quest)
			{
				return 1412021;
			}
			else if(qType == enum_zhixian_quest)
			{
				return 1412022;
			}
			else
				return 1412023;
		}
		break;
	case KQ_QuestRuning:
	case KQ_QuestFailed:
		{
			if(qType == enum_normal_quest)
			{
				return 1412031;
			}
			else if(qType == enum_zhixian_quest)
			{
				return 1412032;
			}
			else
				return 1412033;
		}
		break;
	}
	return 0;
}

DWORD KQuestHolderBase::GetQuestTypeStrID( DWORD qType )
{
	switch (qType)
	{
	case enum_normal_quest:		// 主线任务
		return 1412001;
	case enum_zhixian_quest:	// 支线任务
		return 1412002;
	}
	if (qType > enum_zhixian_quest)
	{
		return 1412003;	// 默认都是活动
	}
	return 0;
}

bool KQuestHolderBase::CanSubmit(int qid)
{
	KQuestNew* pQuest = this->GetActiveQuest(qid);
	if(!pQuest) return false;
	return (pQuest->GetQuestStatus() == KQ_PreStepOver);
}

bool KQuestHolderBase::SubmitQuest( int qid, int sltItemId )
{
	KQuestNew* pQuest = this->GetActiveQuest(qid);
	if(!pQuest) return false;

	if(pQuest->GetQuestStatus() == KQ_PreStepOver)
	{
		return this->QuestOk(pQuest);
	}

	return true;
}



void KQuestHolderBase::SetQuestDone( int qid )
{
	ASSERT_RETURN_VOID(m_pPlayer);
	KQuestNew* pQuest = this->GetActiveQuest(qid);
	if (!pQuest) return;
	pQuest->SetQuestDone();
}



