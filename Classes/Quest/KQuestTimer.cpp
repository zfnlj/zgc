#include "KQuestTimer.h"
#include "KQuestNew.h"
#include "KQuestGroup.h"
#include "KQuestTarget.h"
#include "KQuestParamParser.h"
#include "../WorldObject/KPlayer.h"

KQuestTimer::KQuestTimer()
	: m_active(TRUE)
	, m_totalTicks(0)
	, m_timerType(KQuestTimer::empty_timer)
{

}

KQuestTimer::~KQuestTimer()
{

}

int KQuestTimer::GetRemainTicks()
{
	KQuestNew* pQuest = this->_GetQuest();
	if(!pQuest) return 0;
	int remain = pQuest->GetSession(m_name.c_str(), -1);
	if(remain < 0) return m_totalTicks;
	return remain;
}

KQuestTimer* KQuestTimer::CreateInstance(const char* attrs)
{
	KQuestTimer* pTimer = KQuestTimer::Alloc();
	pTimer->Reset();
	if(!pTimer->Initialize(attrs))
	{
		pTimer->Destroy();
		return NULL;
	}
	return pTimer;
}

KQuestTimer* KQuestTimer::CreateInstance(KQuestNew* pQuest, KQuestTimer& o)
{
	KQuestTimer* pTimer = (KQuestTimer*)o.Clone();
	pTimer->SetOwner(pQuest);
	return pTimer;
}

KQuestTimer* KQuestTimer::CreateInstance(KTargetGroup* pGrp, KQuestTimer& o)
{
	KQuestTimer* pTimer = (KQuestTimer*)o.Clone();
	pTimer->SetOwner(pGrp);
	return pTimer;
}

KQuestTimer* KQuestTimer::CreateInstance(KQuestTarget* pTarget, KQuestTimer& o)
{
	KQuestTimer* pTimer = (KQuestTimer*)o.Clone();
	pTimer->SetOwner(pTarget);
	return pTimer;
}

BOOL KQuestTimer::IsActive() const
{
	return m_active;
}

void KQuestTimer::Reset()
{
	m_active = TRUE;
	m_totalTicks = 0;
	m_name.clear();
	m_timerType = empty_timer;
}

void KQuestTimer::SetOwner(KQuestNew* pQuest)
{
	m_timerType = quest_timer;
	m_pQuestOwner = pQuest;
}

void KQuestTimer::SetOwner(KTargetGroup* pGrp)
{
	m_timerType = group_timer;
	m_pGrpOwner = pGrp;
}

void KQuestTimer::SetOwner(KQuestTarget* pTarget)
{
	m_timerType = target_timer;
	m_pTargetOwner = pTarget;
}

KQuestNew* KQuestTimer::_GetQuest()
{
	switch(m_timerType)
	{
	case quest_timer:
		return m_pQuestOwner;
	case group_timer:
		return m_pGrpOwner->GetOwner();
	case target_timer:
		return m_pTargetOwner->m_pGroup->GetOwner();
	}
	return NULL;
}

const char* KQuestTimer::GetName(void) const
{
	return Target_Timer;
}

bool KQuestTimer::IsOver(void)
{
	return this->GetRemainTicks() < 1;
}

void KQuestTimer::Destroy(void)
{
	this->Reset();
	KQuestTimer::Free(this);
}

KQuestTarget* KQuestTimer::Clone()
{
	KQuestTimer* pTimer = KQuestTimer::Alloc(); pTimer->Reset();
	
	this->CopyTo(pTimer);

	pTimer->m_active = m_active;
	pTimer->m_name = m_name;
	pTimer->m_totalTicks = m_totalTicks;

	return pTimer;
}

bool KQuestTimer::_SetAttr(const char* name, const char* val)
{
	if(strcmp(name, "name") == 0)
	{
		m_name = val;
		return true;
	}
	else if(strcmp(name, "duration") == 0)
	{
		m_totalTicks = str2int(val) * 1000;
		return true;
	}
	else if(strcmp(name, "offline") == 0)
	{
		return true;	// m_offLine属性只在服务器有用, 客户端就直接返回
	}
	else
	{
		return false;
	}
}

int KQuestTimer::_GetIntAttr(const char* name, int defVal)
{
	if(strcmp(name, "total") == 0)
	{
		return m_totalTicks/1000;
	}
	else if(strcmp(name, "count") == 0)
	{
		return this->GetRemainTicks()/1000;
	}
	else
	{
		return defVal;
	}
}

const char* KQuestTimer::_GetStringAttr(const char* name, const char* defVal)
{
	if(strcmp(name, "name") == 0)
	{
		return m_name.c_str();
	}
	else
	{
		return defVal;
	}
}
