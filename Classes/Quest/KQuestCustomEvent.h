#ifndef _K_QUEST_CUSTOM_EVENT_H_
#define _K_QUEST_CUSTOM_EVENT_H_

#include <System/KPlatform.h>
#include <System/Memory/KStepObjectPool.h>
//#include <Event/KEventDefine.h>
#include <System/Collections/KString.h>

class KQuestNew;

struct KQuestCustomEvent : public System::Memory::KPortableMemoryPool<KQuestCustomEvent,1024>
{
	int m_evtId;
	KQuestCustomEvent* m_pNextNode;

	////////////////////////////////////////////////
	KQuestCustomEvent();
	~KQuestCustomEvent();
	bool OnInit(KQuestNew* pQuest);
	void OnUninit(KQuestNew* pQuest);
	void Reset();
	KQuestCustomEvent* Clone();
};

#endif
