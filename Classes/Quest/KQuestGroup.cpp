#include "KQuestGroup.h"
#include "KQuestNew.h"
#include "KQuestParamParser.h"
#include "CommonLogic/Quest/KQuestAttrs.h"
#include <System/Memory/KBuffer64k.h>
#include "KQuestTimer.h"
//#include <PacketGate/KGCPacket.h>
#include "../WorldObject/KPlayer.h"

KTargetGroup::KTargetGroup()
{
	m_grpIndex = 0;
	m_pQuest = NULL;
	m_relation = RELATION_AND;
	m_luaOnEnter = NULL;
	m_luaOnFinish = NULL;
	m_pGrpTimer = NULL;
	m_pNextNode = NULL;
}

KTargetGroup::~KTargetGroup()
{

}

KTargetGroup* KTargetGroup::Clone()
{
	KTargetGroup* pGrp = KTargetGroup::Alloc();

	pGrp->Reset();
	pGrp->m_grpIndex = m_grpIndex;
	pGrp->m_relation = m_relation;
	pGrp->m_luaOnEnter = m_luaOnEnter;
	pGrp->m_luaOnFinish = m_luaOnFinish;
	
	KQuestTarget* pTarget = m_targetList.begin();
	while(pTarget)
	{
		KQuestTarget* pTargetClone = pTarget->Clone();
		pGrp->AddTarget(pTargetClone);

		pTarget = m_targetList.next(pTarget);
	}

	if(m_pGrpTimer)
	{
		pGrp->m_pGrpTimer = KQuestTimer::CreateInstance(pGrp, *m_pGrpTimer);
	}

	return pGrp;
}

bool KTargetGroup::OnGameEvent(int evtId, void* evtData, int len)
{
	bool processed = FALSE;

	//KPlayer * ikp = m_pQuest->GetPlayer();
	//ASSERT(ikp);

	KQuestTarget* pTarget = m_targetList.begin();
	while (pTarget)
	{
		if (pTarget->OnGameEvent(evtId, evtData, len))
		{
			//SC_QuestTargetUpdate packet;
			//packet.grpNo = m_grpIndex;
			//packet.targetNo = pTarget->m_targetIdx;
			//ikp->SyncData(SC_QuestTargetUpdate::s_nCode, &packet, sizeof(packet), eSyncMyself);

			processed = TRUE;
		}
		pTarget = m_targetList.next(pTarget);
	}

	return processed;
}

bool KTargetGroup::OnFinish()
{
	if (!m_pQuest) return false;
	KPlayer* ikp = m_pQuest->GetPlayer();
	if (!ikp) return false;
	KQuestNew* pQuest = m_pQuest;
	if (m_luaOnFinish)
	{
		bool bRetVal = LuaWraper.Call<bool>(m_luaOnFinish, pQuest, this, pQuest->GetPlayer());
		if (!bRetVal) return false;
	}
	else
	{
		KQuestTarget* pTarget = m_targetList.begin();
		while (pTarget)
		{
			pTarget->OnFinish();
			pTarget = m_targetList.next(pTarget);
		}
	}
	// onfinish时注销下当前group注册的事件
	return true;
}


bool KTargetGroup::SetTargetRelation(const char* strRel)
{
	if (_stricmp(strRel, "and") == 0)
	{
		m_relation = RELATION_AND;
		return true;
	}
	else if (_stricmp(strRel, "or") == 0)
	{
		m_relation = RELATION_OR;
		return true;
	}

	return false;
}

KQuestTarget* KTargetGroup::CreateTarget(const char* targetName, const char* paramString)
{
	KQuestTarget* pKQuestTarget = KQuestTarget::Create(targetName);
	if (NULL == pKQuestTarget)
		return NULL;

    if (!pKQuestTarget->Initialize(paramString))
	{
		Log(LOG_ERROR, "error: create quest target %s, params:%s", targetName, paramString);
		pKQuestTarget->Destroy();
		return NULL;
	}

	this->AddTarget(pKQuestTarget);
	return pKQuestTarget;
}

KQuestTimer* KTargetGroup::CreateTimer(const char* attrs)
{
	ASSERT(!m_pGrpTimer);

	m_pGrpTimer = KQuestTimer::CreateInstance(attrs);
	ASSERT(m_pGrpTimer);
	
	m_pGrpTimer->SetOwner(this);
	return m_pGrpTimer;
}

KQuestNew* KTargetGroup::GetQuest()
{
	return m_pQuest;
}

int KTargetGroup::GetGroupIndex()
{
	return m_grpIndex;
}

KQuestTarget* KTargetGroup::GetTarget(int targetIdx)
{
	int c = m_targetList.size();
	if (targetIdx < 0 || targetIdx >= c) return NULL;

	KQuestTarget* pTarget = m_targetList.begin();
	while (targetIdx)
	{
		pTarget = m_targetList.next(pTarget);
		targetIdx --;
	}
	return pTarget;
}

void KTargetGroup::Reset()
{
	m_grpIndex = 0;
	m_pQuest = NULL;
	m_relation = RELATION_AND;
	m_luaOnEnter = NULL;
	m_luaOnFinish = NULL;

	while (!m_targetList.empty())
	{
		KQuestTarget* pTarget = m_targetList.pop_front();
		pTarget->Destroy();
	}

	if(m_pGrpTimer)
	{
		m_pGrpTimer->Reset();
		KQuestTimer::Free(m_pGrpTimer);
		m_pGrpTimer = NULL;
	}
}

KQuestNew* KTargetGroup::GetOwner()
{
	return m_pQuest;
}

bool KTargetGroup::IsOver()
{
	KQuestTarget* pTarget = NULL;

    // Target`s relation is or.
	if (m_relation == RELATION_OR)
	{
		for (pTarget=m_targetList.begin(); pTarget; pTarget=m_targetList.next(pTarget))
		{
			if (pTarget->IsOver())
			{
				return true;
			}
		}

		return false;
	}

	// Target`s relation is and.
    for (pTarget=m_targetList.begin(); pTarget; pTarget=m_targetList.next(pTarget))
	{
		if (!pTarget->IsOver())
		{
			return false;
		}
	}

	return true;
}

bool KTargetGroup::HasTarget(const char* targetName)
{
	KQuestTarget* pTarget = m_targetList.begin();
	while(pTarget)
	{
		if(_stricmp(pTarget->GetName(), targetName) == 0)
			return true;
		pTarget = m_targetList.next(pTarget);
	}
	return false;
}

int KTargetGroup::GetTraceText(char* buf, int len)
{
	KQuestTarget* pTarget = m_targetList.begin();

	if(m_pGrpTimer && m_pGrpTimer->IsActive())
	{
		int timerlen = m_pGrpTimer->GetTraceText(buf, len);
		if(timerlen)
		{
			buf += timerlen; 
			*buf++ = '<';
			*buf++ = 'b';
			*buf++ = 'r';
			*buf++ = '>';
			len -= timerlen + 4;
		}
	}
	
	char* p = buf;
	while(pTarget)
	{
		int l = pTarget->GetTraceText(p, len);
		pTarget = m_targetList.next(pTarget);
		if(l > 0) 
		{
			p[l++] = '<';
			p[l++] = 'b';
			p[l++] = 'r';
			p[l++] = '>';
		}
		p += l; len -= l;
	}
	return (int)(p-buf);
}

LuaString KTargetGroup::GetTraceText_lua()
{
	char buf[512];
	int len = this->GetTraceText(buf, 512);
	return LuaString(buf, len);
}

void KTargetGroup::AddTarget(KQuestTarget* pTarget)
{
	pTarget->OnAddToGroup(this);
	m_targetList.push_back(pTarget);
}
