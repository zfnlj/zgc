#ifndef _K_QUEST_GROUP_H_
#define _K_QUEST_GROUP_H_

#include <System/KPlatform.h>
#include <System/Collections/KSDList.h>
#include <System/Memory/KStepObjectPool.h>
#include "KQuestTarget.h"
#include "Inc/Lua/KLuaWrap.h"

class KQuestNew;
class KQuestTimer;

//-----------------------------------------------------------------
// Target group中的所有目标只要有一个完成了，就认为整个组完成了
// 所以，组里面的目标之间的关系是 '或'
// 在一个任务步骤中，必须先完成前面的组，才能开始处理后面的组
// 即，组的顺序代表了时间的顺序
//-----------------------------------------------------------------
class KTargetGroup
	: public System::Memory::KPortableMemoryPool<KTargetGroup,2048>
{
public:
	enum enumRelation
	{
		RELATION_OR,
		RELATION_AND,
	};

public:
	KTargetGroup();
	~KTargetGroup();

public:
	KTargetGroup* Clone();

	bool SetTargetRelation(const char* strRel);
	KQuestTarget* CreateTarget(const char* targetName, const char* paramString);
	KQuestTimer* CreateTimer(const char* attrs);

	KQuestNew* GetQuest();
	int GetGroupIndex();

	KQuestTarget* GetTarget(int targetIdx);

	void Reset();
	bool OnGameEvent(int evtId, void* evtData, int len);
	bool OnFinish();
public:
	KQuestNew* GetOwner();
	bool IsOver();

	bool HasTarget(const char* targetName);
	int GetTraceText(char* buf, int len);
	LuaString GetTraceText_lua();

private:
	void AddTarget(KQuestTarget* pTarget);

public:
	int m_grpIndex;
	KQuestNew* m_pQuest;
	enumRelation m_relation;
	char* m_luaOnEnter;
	char* m_luaOnFinish;
	System::Collections::KSDSelfList<KQuestTarget> m_targetList;

public://timer
	KQuestTimer* m_pGrpTimer;

public:
	KTargetGroup* m_pNextNode;

////////////////////////////////////任务成就相关////////////////////////////////////////
public:
	void SetQuestAchv(int idx) { return; }	// 为了脚本能编过

public:
	BeginDefLuaClass(KTargetGroup);
		DefMemberFunc(CreateTarget);
		DefMemberFunc(GetQuest);
		DefMemberFunc(GetGroupIndex);
		DefMemberFunc(GetTarget);
		DefMemberFunc(IsOver);
		DefMemberFunc(SetQuestAchv);
		DefMemberFunc2(GetTraceText_lua, "GetTraceText");
	EndDef;
};

#endif
