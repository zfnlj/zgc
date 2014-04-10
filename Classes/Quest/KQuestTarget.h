
#ifndef _K_QUEST_TARGET_H_
#define _K_QUEST_TARGET_H_

#include <System/KPlatform.h>
#include <System/Memory/KStepObjectPool.h>
#include <System/Collections/DynArray.h>
#include "KQuestParamParser.h"
#include "Inc/Lua/KLuaWrap.h"
#include "CommonLogic/Quest/KQuestPersist.h"

// Target name.
const char Target_NpcDialog			[] = "NpcDialog";			// ������ɶԻ�
const char Target_Dialog			[] = "Dialog";				// ��������еĶԻ�
const char Target_MapArrive			[] = "ArriveMap";
const char Target_QuestOver			[] = "QuestOver";
const char Target_LuaImp			[] = "LuaImp";
const char Target_LuaExt			[] = "LuaExt";
const char Target_PickItem			[] = "PickItem";
const char Target_LevelUp			[] = "LevelUp";
const char Target_GetItem			[] = "GetItem";
const char Target_DemandDialog		[] = "DemandDialog";		// ��Ҫ���� derive from Target_Dialog
const char Target_UseItem			[] = "UseItem";				// ʹ�õ�������
const char Target_KillPlayer		[] = "KillPlayer";			// ��ɱ���
const char Target_KillMonster		[] = "KillMonster";
const char Target_PlayerOperate		[] = "PlayerOperate";		// ������ĳ��������紴��������ѧϰ���� 
const char Target_LuaCount			[] = "LuaCount";			// �ű���������, ��TakeOneStep�ƶ�����������, ��ȻҲ���ڽű�������ע���ƶ��¼�

// �����Target����KQuestTimerʵ��
const char Target_Timer				[] = "Timer";

namespace KWorldObjAbout
{
	class KPlayer;
}

class KQuestNew;
class KTargetGroup;
class KQuestTimer;

class KQuestTarget
{
public:
	static KQuestTarget* Create(const char* typeName);

public:
	KQuestTimer* CreateTimer(const char* attrs);

public:
	KQuestTarget();
	virtual ~KQuestTarget(void);

	virtual const char* GetName(void) const			{ ASSERT(FALSE); return NULL; }
	virtual bool Initialize(const char* paramStr);
	virtual KQuestTarget* Clone()					{ ASSERT(FALSE); return NULL; }

	virtual bool IsOver(void);
	virtual void Reset(void);
	virtual void Destroy(void);
	void OnFinish();
	virtual void _OnFinish();
	virtual int GetTraceText(char* buf, int len);
	virtual LuaString GetTraceText_lua();

	bool IsKindOf(const char* targetName);
	KTargetGroup* GetGroup();
	const char* Topname();
	const char* Tonpc();
	LuaString ToPosName(const char* name);

	LuaString ToNpcName(DWORD npcTypeId);

	bool SetAttr(const char* name, const char* val);
	int GetIntAttr(const char* name, int defVal);
	const char* GetStringAttr(const char* name, const char* defVal);

	// �õ�target��CounterName, ��������ʵ��
	LuaString GetTargetCounterName();

	// ������ʵ�ֵ��������ԣ����ù������ڻ��������
	virtual bool _SetAttr(const char* name, const char* val)					{ return FALSE;  }
	virtual int _GetIntAttr(const char* name, int defVal)						{ return defVal; }
	virtual const char* _GetStringAttr(const char* name, const char* defVal)	{ return defVal; }
	virtual LuaString GetCounterName() { return LuaString(""); }

	int GetQuestType();	// �õ�target������������
	KQuestNew* GetQuest();
	virtual KQuestNew* _GetQuest() { return NULL; }
	KWorldObjAbout::KPlayer* GetPlayer();

	virtual void SetOver(void);

	virtual void OnAddToGroup(KTargetGroup* pGrp);
	virtual bool OnGameEvent(int evtId, void* evtData, int len);
	virtual bool _OnGameEvent(int evtId, void* evtData, int len);
protected:
	void Destroy_Delete(void);
	int _GetTraceText(char* buf, int len);
	bool ReadParams(KQuestParamParser& parser);
	void CopyTo(KQuestTarget* pTarget);
public:
	int m_targetIdx;
	DWORD m_traceId;
	int m_location;   // Ѱ·��Ϣ
	KTargetGroup* m_pGroup;

	static char* m_buffer;
public://timer
	KQuestTimer* m_pTargetTimer;

public:
	KQuestTarget* m_pNextNode;

public:
	void target_InitLuaFlag(const char* targetTableName){}	// ���ݷ������ű�

public:
	BeginDefLuaClass(KQuestTarget);
		DefMemberFunc(Initialize);
		DefMemberFunc(IsKindOf);
		DefMemberFunc(GetGroup);
		DefMemberFunc(Topname);
		DefMemberFunc(Tonpc);
		DefMemberFunc(ToPosName);
		DefMemberFunc(ToNpcName);
		DefMemberFunc(target_InitLuaFlag);
		DefMemberFunc(SetAttr);
		DefMemberFunc(GetIntAttr);
		DefMemberFunc(GetStringAttr);
		DefMemberFunc(GetQuest);
		DefMemberFunc(GetPlayer);
		DefMemberFunc(SetOver);
		DefMemberFunc(IsOver);
		DefMemberFunc(GetQuestType);
		DefMemberFunc(GetTargetCounterName);
		DefMemberFunc2(GetTraceText_lua,"GetTraceText");
	EndDef;
};

// dialog with NPC, submit quest.
// "npc=20001"
class KTarget_NpcDialog
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_NpcDialog,2048>
{
public:
	KTarget_NpcDialog(void);
	~KTarget_NpcDialog(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone();

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);

public:
	DWORD m_npcId;
};


// quest=<questId>;reason=<0,1,2>
class KTarget_QuestOver
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_QuestOver,2048>
{
public:
	KTarget_QuestOver();
	~KTarget_QuestOver();

public:
	const char* GetName() const;
	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);
	bool IsOver(void);

public:
	KQuestTarget* Clone();
	void Reset(void);
	void Destroy(void);
private:
	LuaString GetCounterName();
public:
	int m_qid;
	int m_qType;
	int m_reason;
	int m_total;
};

// get item.
// "item=101;count=10"
class KTarget_GetItem
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_GetItem,2048>
{
public:
	KTarget_GetItem(void);
	~KTarget_GetItem(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone();

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);

private:
	bool IsCared(DWORD itemId);
	LuaString GetCounterName();

public:
	int m_total;
	int m_itemCount;
	DWORD m_itemIds[10];
};

// Level up
// "level=10"
class KTarget_LevelUp
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_LevelUp,2048>
{
public:
	KTarget_LevelUp(void);
	~KTarget_LevelUp(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone(void);

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);

public:
	int m_level;
};


// use item.
// "item=101;obj=10;adr=1400000"
class KTarget_UseItem
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_UseItem,2048>
{
public:
	KTarget_UseItem(void);
	~KTarget_UseItem(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone();

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);

private:
	bool IsCared(DWORD itemId);
	LuaString GetCounterName();

public:
	int m_adr;		// ʹ�õ��ߵص����Ƶ�stringid
	int m_x;		// ����ʹ�õص�����x
	int m_y;		// ����ʹ�õص�����y
	int m_npcId;	// ʹ�õ��߶����npcTypeId --- npc
	int m_objId;	// ʹ�õ��߶����obj����id,��location�ṩѰ·
	int m_objType;	// ʹ�õ��߶�������1Ϊnpc,2Ϊ����,3Ϊ�ؾ�
	DWORD m_itemId; // ÿ��targetֻ��עһ��item
	int m_total;	// ʹ�ô���
};

class KTarget_PlayerOperate
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_PlayerOperate,2048>
{
public:
	KTarget_PlayerOperate(void);
	~KTarget_PlayerOperate(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone();

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);

private:
	LuaString GetCounterName();
public:
	int m_opType;	// ��������
	int m_opParam;	// ��������
};

class KTarget_Dialog
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_Dialog,2048>
{
public:
	KTarget_Dialog(void);
	~KTarget_Dialog(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone();

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);

private:
	bool IsCared(DWORD npcId);
	LuaString GetCounterName();

public:
	int m_npcCount;
	DWORD m_npcIds[10];
	int m_dialogIdx;
};

// kill monster.
// "npc=10000,20000;count=10"
class KTarget_KillMonster
	: public KQuestTarget
	, public System::Memory::KPortableMemoryPool<KTarget_KillMonster,2048>
{
public:
	KTarget_KillMonster(void);
	~KTarget_KillMonster(void);

public:
	const char* GetName(void) const;
	KQuestTarget* Clone();

	bool _SetAttr(const char* name, const char* val);
	int _GetIntAttr(const char* name, int defVal);

	bool IsOver(void);
	void Reset(void);
	void Destroy(void);
	bool _OnGameEvent(int evtId, void* evtData, int len);
	virtual void _OnFinish();
private:
	bool IsCared(DWORD villageId);
	LuaString GetCounterName();

public:
	int m_total;
	int m_npcCount;
	DWORD m_npcIds[10];
};

#endif
