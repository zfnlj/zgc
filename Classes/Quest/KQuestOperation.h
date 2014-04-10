#ifndef _K_QUEST_OPERATION_H_
#define _K_QUEST_OPERATION_H_

#include <System/KPlatform.h>
#include <System/Memory/KStepObjectPool.h>
#include "../EventSystem/KEventDefine.h"
#include <System/Collections/KString.h>
#include "KQuestNew.h"
#include "KQuestData_S_i.h"
#include "KPlayerQuestManager.h"
//#include "KQuestData_S_i.h"
//#include "../Interactive/KInteractiveItem.h"
//#include "../Interactive/KInteractiveManager.h"

//#include "KChatTypeDefine.h"

class KQuestPtrCmp
{
public:
	typedef KQuestNew* KQuestPtr;
	bool operator () (const KQuestPtr& a, const KQuestPtr& b) const { return a->m_qid < b->m_qid; }
};
typedef System::Collections::DynSortedArray<KQuestNew*,KQuestPtrCmp> QuestArray;

class KQuestHolderBase
{
public:
	KQuestHolderBase();
	KQuestHolderBase(KPlayer * pPlayer){m_pPlayer = pPlayer;};
	~KQuestHolderBase();

	void Breathe(int nInterval);
public:
	bool AcceptQuest(int qid);
	bool AcceptQuestForce(int qid);
	bool _AcceptQuestForce(KQuestNew* pQuest);
public:
	// �������ͻ���ͨѶ 
	void OnQuestAttr(int qid, int attrId, const LuaString& val);
	void OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val);
public: // quest about
	KQuestNew* GetActiveQuest(int qid);
	bool IsQuestEverDone(int qid);
	bool IsQuestEverDone(int qid, int dayOffset);
	bool GetQuestHistory(int qid, int& times, time_t& lastDo);
public:
	bool QuestOk(KQuestNew* pQues);
	bool SubmitQuest(int qid, int sltItemId);
	bool CancelQuest(KQuestNew* pQuest);
	bool CanSubmit(int qid);

public:
	////////////////////////////////////������ӿ�//////////////////////////////////////

	//������
	KQuestNew* AddQuest(int qid);
	void SetQuestDone(int qid);
	//ɾ������
	bool RemoveQuest(int qid);
	
	//�Ƿ���ڸ�����
	bool HasQuest(DWORD qid);
	
private:
	KPlayer * m_pPlayer;
	KPlayerQuestManager::QuestSortArray& _GetPlayerQuestArray();
	KPlayerQuestManager::QuestSortArray& _GetPlayerActiveQuestArray();
	DWORD GetQuestViewIconStrID(DWORD qState, DWORD qType);	// ��������״̬���������͵õ�����ͼ��stringid
	DWORD GetQuestTypeStrID(DWORD qType);	// �õ�����״̬����ʾ�ַ���id
};


class KQuestOperation
{
	KQuestOperation();
	~KQuestOperation();

};

#endif
