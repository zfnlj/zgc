#ifndef _K_QUEST_TIMER_H_
#define _K_QUEST_TIMER_H_

#include <System/Memory/KStepObjectPool.h>
#include <System/Collections/KString.h>
#include "KQuestTarget.h"

class KQuestNew;
class KTargetGroup;
class KQuestTarget;

/**
 * 从KQuestTarget继承是为了获得定制跟踪文字的功能.
 * Timer对象本身并不是一个Target.
 */
class KQuestTimer
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KQuestTimer,8>
{
public:
	enum enumTimerType
	{
		empty_timer,
		quest_timer,
		group_timer,
		target_timer,
	};

public:
	KQuestTimer();
	~KQuestTimer();

public:
	int GetRemainTicks();

public:
	// name=xx-timer;duration=100;trace=100
	static KQuestTimer* CreateInstance(const char* attrs);
	static KQuestTimer* CreateInstance(KQuestNew* pQuest, KQuestTimer& o);
	static KQuestTimer* CreateInstance(KTargetGroup* pGrp, KQuestTimer& o);
	static KQuestTimer* CreateInstance(KQuestTarget* pTarget, KQuestTimer& o);

public:
	BOOL IsActive() const;
	virtual KQuestNew* _GetQuest();
	void Reset();

public:
	void SetOwner(KQuestNew* pQuest);
	void SetOwner(KTargetGroup* pGrp);
	void SetOwner(KQuestTarget* pTarget);

public:// 从KQuestTarget继承
	const char* GetName(void) const;
	bool IsOver(void);
	void Destroy(void);
	KQuestTarget* Clone();
	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);
	const char* _GetStringAttr(const char* name, const char* defVal);
	
public:
	BOOL m_active;
	int m_totalTicks;
	::System::Collections::KString<64> m_name;

	enumTimerType m_timerType;
	union
	{
		KQuestNew* m_pQuestOwner;
		KTargetGroup* m_pGrpOwner;
		KQuestTarget* m_pTargetOwner;
	};
public:
	BeginDefLuaClass(KQuestTimer)
		InheritFrom(KQuestTarget);
	EndDef
};

#endif
