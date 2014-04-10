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
// Target group�е�����Ŀ��ֻҪ��һ������ˣ�����Ϊ�����������
// ���ԣ��������Ŀ��֮��Ĺ�ϵ�� '��'
// ��һ���������У����������ǰ����飬���ܿ�ʼ����������
// �������˳�������ʱ���˳��
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

////////////////////////////////////����ɾ����////////////////////////////////////////
public:
	void SetQuestAchv(int idx) { return; }	// Ϊ�˽ű��ܱ��

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
