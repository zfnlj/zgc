#pragma once

#include "KQuestNew.h"
#include <System/Collections/KMapByVector.h>
#include <System/Collections/KTree.h>
#include <System/Memory/KAlloc.h>
#include "Inc/Lua/KLuaWrap.h"
#include "KQuestParamParser.h"
#include "CommonLogic/Quest/KQuestArray.h"


// δ���������
struct AvailQuestDesc
{
	int m_nQuestId;
	int m_nLevel;
	int m_nMinLevelLimit;
	int m_nMaxLevelLimit;
};

class KQuestManager
{
public:
	typedef System::Collections::KMapByVector<int,KQuestNew*> QuestMap;
	typedef System::Collections::KMapByTree_pooled<DWORD,quest_array_header*,256> NpcQuestMap;
	typedef System::Collections::KMapNode<int,AvailQuestDesc> AvailQuestNode;
	typedef System::Memory::KStepBlockAlloc<AvailQuestNode,256> AvailQuestAlloc;
	typedef System::Collections::KMapByVector<int,AvailQuestDesc,KLess<int>,AvailQuestAlloc> AvailQuestMap;
public:
	KQuestManager(void);
	~KQuestManager(void);

public:
	static KQuestManager* GetInstance();

	bool syncAvailQuests();
private:
	static bool m_initFlag;	// questmanager��ʼ����־, ��ö�γ�ʼ��

public:
	// ��ʼ�����������
	bool Initialize(void);
	void Breathe(int interval);

public:
	KQuestNew* GetQuest(int id);	// ͨ������IDȡ������ָ��
	bool ReloadQuest(int qid);		// ���¼�������ű�
	int doDirQuest(const char* resGrpName);
	bool LoadAvailQuestList(void);
	void LoadLuaScript(const char* fileName);
	bool LoadDailyQuestMap(void);
	KQuestNew* RndDailyQuest(int hardLev);
public:
	void GetFigureAward(KDString<512>& GiftStr, KQuestNew* pQuest);

public:

public:
	quest_array_header* get_quest_array(DWORD npcTypeID);
	NpcQuestMap m_npcQuestMap;
	AvailQuestMap m_availQuestMap;
	QuestMap m_dailyQuestMap;
public:
	QuestMap m_mapQuest;
	KQuestLuaParser m_luaParser;

	// ����̬�Ի����
public:
	bool IsDynTalkQuest(int qid);
	bool IsDynTalkQuest(KQuestNew* pQuest);
	

public:
	BeginDefLuaClass(KQuestManager)
		DefMemberFunc(GetQuest)
	EndDef
};