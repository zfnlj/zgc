#ifndef _K_QUEST_NEW_H_
#define _K_QUEST_NEW_H_

#include <System/KPlatform.h>
#include "KQuestGroup.h"
#include "CommonLogic/Quest/KQuestSession.h"
#include <System/Memory/KStepObjectPool.h>
#include "Inc/Lua/KLuaWrap.h"
#include "CommonLogic/Quest/KQuestAttrs.h"
#include <System/Collections/DynArray.h>
#include "KQuestCustomEvent.h"
#include <System/Collections/KSDList.h>
#include "FQuest.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include "../EventSystem/KEventDefine.h"
namespace KWorldObjAbout
{
	class KPlayer;
}

class KQuestTimer;
using namespace KWorldObjAbout;

class KQuestNew
	: public KFacadeImpl<FQuest,KQuestNew>
	, public System::Memory::KPortableMemoryPool<KQuestNew,128>
{
public:
	enum
	{
		lua_Initialize,
		lua_AfterInitialize,
		lua_GiftDesc,
		lua_GetTraceText,
		lua_Name,
		lua_Synposis,
		lua_Description,
		lua_TalkDesc,
		lua_FinishDesc,
		lua_NoFinishDesc,
		lua_CanSee,
		lua_CanDoThis,
		lua_Count,
	};

	struct EvtReg
	{
		int evtId;
		int refCount;
	};
	
	class evtCmp
	{
	public:
		int operator () (const EvtReg& a, const EvtReg& b) const
		{
			return a.evtId - b.evtId;
		}
	};

	typedef System::Collections::KString<128> MethodName;
	struct ItemDesc { DWORD itemId; int count; };
	typedef System::Collections::KSDSelfList<KQuestCustomEvent> CustomEventList;
	typedef System::Collections::KDynSortedArray<EvtReg,evtCmp,16,16> EvtRegArray;

public:
	KQuestNew();
	~KQuestNew();

public:
	static bool IsDailyQuest(BYTE qtype);

public:
	// ��Lua�г�ʼ��
	bool Initialize(int qid, const char* questTable=NULL);
	bool AfterInitialize(const char* questTable);

public:
	int Breathe(int interval);
	DWORD m_lastBreatheTick;

public:
	void SetQuestStatus(KQuestStatusCode status);
	KQuestStatusCode GetQuestStatus();
	KQuestStatusCode m_status;
	void SetQuestDone();
public:
	KQuestNew* Clone();
	void Reset();

public:// �¼�ע��
	bool RegEvent(int evtId, DWORD key, DWORD customData);
	bool RemoveEvent(int evtId, DWORD key);
	
public:
	// name1=val1;name2=val2; ... ...
	// namei��Ӧ��Excel�����������
	// ��Ҫ��Ϊ lua �ṩ�ӿ�
	bool SetAttrs(const char* attrStr);
	KTargetGroup* CreateTargetGroup(const char* rel);
	bool CreateUserEvent(int evtId);
	KQuestTimer* CreateTimer(const char* attrs);
	KQuestTimer* GetCurTimer();	// �õ�����ǰ��ʱ��

	bool SetCurrentGroup(int grpIndex);
	int GetCurrentGroup(void);

public: // quest flags enum_quest_flag
	bool SetFlag(int which);
	void ClearFlag(int which);
	bool HasFlag(int which) const;

public:
	KPlayer* GetPlayer();
	KPlayer* GetOwner();
	int GetGroupCount() const;
	KTargetGroup* GetGroup(int grpIdx);

public: // session about
	KQuestSession* GetSessionObj() ;
	int GetSession2(DWORD key, int defVal);
	int GetSession(const char* name, int defVal);

	bool SetSession(const char* name, int val);
	bool SetSession2(DWORD key, int val);

	int IncSession(const char* name, int delta);
	int DecSession(const char* name, int delta);
	void RemoveSession(const char* name);
public:
	bool IsTraceMode();
	bool IsAbandonAble(){ return m_abandon>0;}
	int GetIntAttr(const char* attrName, int defVal);
	const char* GetStringAttr(const char* attrName, const char* defVal);
	
	const char* GetTraceText(char* buf, int len);

	const char* GetName(KPlayer* pPlayer, char* buf, int len);
	const char* GetDesc(KPlayer* pPlayer, char* buf, int len);
	const char* GetStatusDesc(KPlayer* pPlayer, char* buf, int len);
	void GetGiftDesc(KPlayer* pPlayer, KDString<512>& GiftStr);
	void GetItemGiftDesc(KPlayer* pPlayer, KDString<512>& GiftStr);
	void GetGiftDescWithExtraExp(KPlayer* pPlayer, KDString<512>& GiftStr, int aExp, int aTimes);
	void GetLimitCondition(KPlayer* pPlayer, KDString<512>& cStr);	// �õ��������������
	const char* GetTalkDesc(KPlayer* pPlayer, char* buf, int len);
	const char* GetNoFinishDesc(KPlayer* pPlayer, char* buf, int len);
	const char* GetFinishDesc(KPlayer* pPlayer, char* buf, int len);
	const char* GetTypeText(KPlayer* pPlayer, char* buf, int len);
	const char* GetTypeText_lua(int qtype = 0);

	bool IsOver();
	int GetID(){ return m_qid;}
public:
	int GetStatDescriptionID(KPlayer* iPlayer, DWORD dwQuestState);
	const char* _GetTraceText(char* buf, int len);
	bool SetAttr(int attrId, char* val);
	bool IsOverStatus();

private:
	void AddGroup(KTargetGroup* pGrp);
	LuaString _GetName(KPlayer* pPlayer);
	LuaString _GetColorName(KPlayer* pPlayer);

public:
	// �ͻ���Npc����״̬���
	bool CanSee(KPlayer* pPlayer, DWORD npcId);
	bool CanDoThis(KPlayer* pPlayer);
	bool checkPeriod(KPlayer* pPlayer);
public:
	int   m_qid;

	DWORD m_nameId;
	DWORD m_talkId;
	DWORD m_acceptDialogId;		// ���ܶԻ�
	DWORD m_finishDialogId;		// ��ɶԻ�
	int   m_level;
	int   m_acceptLevel;
	int	  m_acceptTopLimit;		// ����ȼ�����
	int   m_prevQuest;
	int   m_nextQuest;

	int	  m_profession;	// �������ְҵ

	int   m_grade;		// ����Ƚף���ֵ�����������ɫ

	BYTE  m_type;
	//BYTE  m_view;
	BYTE  m_abandon;
	BYTE  m_reuse;
	BYTE  m_share;

	// �����־
	// bit0. �Ƿ�ɻ���
	DWORD m_dwFlags;

	// �������ƹ�����
	int m_hardDegree;
	int m_money;
	int m_exp;
	int m_expBase;
	DWORD m_descId;
	DWORD m_finishDescId;
	DWORD m_noFinishDescId;

	int m_achieveId;
	int m_achievement;

	int m_extraDescStrId;	// �������������ַ���id
	int m_extraDescBGPId;	// ��������������ԴͼƬ�ַ���id, Ҳ��stringid

	int m_battleField;

	int m_timeperiod;

	BYTE m_itemGiftCount;
	ItemDesc m_itemGifts[4];

	
	BYTE m_questItemCount;
	ItemDesc m_questItems[4];

	DWORD m_dwLuaMethodFlag;

	KPlayer* m_pPlayer;
	KQuestSession* m_pSession;

	KTargetGroup* m_pCurrGrp;
	System::Collections::KSDSelfList<KTargetGroup> m_grpList;
	System::Collections::KSDSelfList<KQuestCustomEvent> m_evtList;

	EvtRegArray m_evtArray;
public:// timer
	KQuestTimer* m_pQuestTimer;

public:
	static const char* m_luaMethods[lua_Count];
	void InitLuaFuncFlag(const char* tableName);
	bool hasLua(int method);
	MethodName luaMethod(int method);
	
	void OnFinish();
	void OnGift();
	void OnGameEvent(int evtId, void* evtData, int len);
	virtual bool OnEventImp(KEventAbout::KEventID id, const KEventAbout::KEventData* pData, uint_r customData);
////////////////////////////////////����ɾ����////////////////////////////////////////
public:
	void CreateAchieve(const char* typeName, const char* paramStr) { return; }	// Ϊ�˽ű�������

public:
	BeginDefLuaClass(KQuestNew)
		DefMemberFunc(SetFlag);
		DefMemberFunc(ClearFlag);
		DefMemberFunc(HasFlag);
		DefMemberFunc(GetPlayer)
 		DefMemberFunc(SetAttrs)
		DefMemberFunc(CreateTargetGroup)
 		DefMemberFunc(CreateUserEvent)
 		DefMemberFunc(GetGroup)
 		DefMemberFunc(GetGroupCount)
 		DefMemberFunc(GetSession)
 		DefMemberFunc(SetSession)
 		DefMemberFunc(IncSession)
 		DefMemberFunc(DecSession)
		DefMemberFunc(RemoveSession)
 		DefMemberFunc(GetIntAttr)
 		DefMemberFunc(GetStringAttr)
		DefMemberFunc(SetCurrentGroup)
		DefMemberFunc(GetCurrentGroup)
		DefMemberFunc(GetTypeText_lua)
		DefMemberFunc(CreateAchieve);
		DefMemberFunc(CreateTimer);
		DefMemberFunc(GetCurTimer);
	EndDef
};

DECLARE_SIMPLE_TYPE(KQuestNew::EvtReg);
#endif
