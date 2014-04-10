#include "KQuestCustomEvent.h"
#include "KQuestNew.h"

KQuestCustomEvent::KQuestCustomEvent()
	: m_evtId(0)
{

}

KQuestCustomEvent::~KQuestCustomEvent()
{

}

bool KQuestCustomEvent::OnInit(KQuestNew* pQuest)
{
	return pQuest->RegEvent(m_evtId, (DWORD)pQuest, 0);
}

void KQuestCustomEvent::OnUninit(KQuestNew* pQuest)
{
	pQuest->RemoveEvent(m_evtId, (DWORD)pQuest);
}

void KQuestCustomEvent::Reset()
{
	m_evtId = 0;
}

KQuestCustomEvent* KQuestCustomEvent::Clone()
{
	KQuestCustomEvent* p = KQuestCustomEvent::Alloc();
	p->Reset();
	p->m_evtId = m_evtId;
	return p;
}
