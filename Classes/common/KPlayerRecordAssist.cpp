#include "KPlayerRecordAssist.h"
#include "../sqlite/KUserSql.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "../Quest/KQuestManager.h"
#include "../Quest/KPlayerQuestManager.h"
#include "../Quest/KClientQuestSession.h"
#include "../Quest/KQuestNew.h"

using namespace KItemAbout;
namespace KPlayerRecordAssist
{

bool syncBagToRecord(KWorldObjAbout::KPlayer* player,tb_player_record* record)
{
	KItemAbout::KBagManager* pBagMgr = player->GetBagManagerPtr();
	if(!pBagMgr) return false;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return false;
	record->normalItem.Set(NULL,0);
	KBag::CellMapVec::const_iterator itorTmp = pBag->m_ListItem.begin();
	for(;itorTmp!=pBag->m_ListItem.end();itorTmp++)
	{
		const KCellBag* pCell = (const KCellBag*)(itorTmp->second);
		KDBBagItemUnit dbItem(pCell->GetItem().GetID(),pCell->GetItem().GetStackNumber());
		record->normalItem.Append(&dbItem,sizeof(dbItem));
	}
	
	return KUserSql::UpdateNormalBag( player->GetName(),record);
}

bool addStoreCard(tb_player_record* record,int id,int count)
{
	void* buf=NULL;
	int len = record->cardStore.Get(buf);
	KDBBagItemUnit* dbCard = (KDBBagItemUnit*)buf;
	int n = len/sizeof(KDBBagItemUnit);
	bool bFound = false;
	for(int i=0;i<n;i++){
		if(dbCard->_id==id){
			dbCard->_count += count;
			bFound = true;
			break;
		}
		dbCard++;
	}
	if(!bFound){
		KDBBagItemUnit addCard(id,count);
		record->cardStore.Append(&addCard,sizeof(addCard));
	}
	record->updateMask(tb_player_record::_CARDSTORE);
	return true;
}

bool syncBagFromRecord(KWorldObjAbout::KPlayer* player,tb_player_record* record)
{
	void* buf;
	int len = record->normalItem.Get(buf);
	KDBBagItemUnit* dbItem=(KDBBagItemUnit*)buf;
	int num = len/sizeof(KDBBagItemUnit);
	
	KItemAbout::KBagManager* pBagMgr = player->GetBagManagerPtr();
	if(!pBagMgr) return false;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return false;

	for(int i=0;i<num;i++,dbItem++){
		pBag->Add(dbItem->_id,dbItem->_count);
	}
	return true;
}

bool syncQuestFromRecord(KPlayerQuestManager* playerQuestMgr,tb_playerquest_record* record)
{
	playerQuestMgr->Reset();
	for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++){
		int qid = record->qid[i];
		KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qid);
		if(!pQuest) continue;
		KQuestNew* pAdd =playerQuestMgr->AddQuest(pQuest);
		void* buf;
		int len = record->qstate[i].Get(buf);
		int num = len/sizeof(KQuestSessionItem);
		KQuestSessionItem * pSessionItem = (KQuestSessionItem*)buf;
		KClientQuestSession* pSession = (KClientQuestSession*)pAdd->m_pSession;
		pSession->InitSessions(pSessionItem,num);
		pAdd->SyncStatusFromSession();
	}
	void* buf;
	int num = record->qhistory.Get(buf)/sizeof(KDBQuestHistoryDataUnit);
	for(int i=0;i<num;i++){
		KDBQuestHistoryDataUnit* pHistory = (KDBQuestHistoryDataUnit*)buf;
		playerQuestMgr->SetQuestHistory(pHistory->qid,pHistory->val,0);
	}
	return true;
}

void ClearHistory(tb_playerquest_record* record,int qid)
{

}

bool AddQuest(tb_playerquest_record* record,KQuestNew* pQuest)
{
	int emptySlot = -1;
	for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++){
		if(record->qid[i]==0){
			emptySlot = i;
			break;
		}
	}
	char buf[160];
	if(emptySlot<0) return false;
	record->qid[emptySlot] = pQuest->m_qid;
	KClientQuestSession* pClientSession = (KClientQuestSession*)pQuest->GetSessionObj();
	int len = pClientSession->QueryData(buf);
	record->qstate[emptySlot].Set(buf,len);
	record->updateMask(BIT(emptySlot*2));
	record->updateMask(BIT(emptySlot*2+1));
	return true;
}

void UpdataQuestSession(tb_playerquest_record* record,KQuestNew* pQuest)
{
	int slot = record->getQuestSlot(pQuest->m_qid);
	if(slot<0) return;
	char buf[160];
	KClientQuestSession* pClientSession = (KClientQuestSession*)pQuest->GetSessionObj();
	int len = pClientSession->QueryData(buf);
	record->qstate[slot].Set(buf,len);
	record->updateMask(BIT(slot*2));
	record->updateMask(BIT(slot*2+1));
}

bool QuestOk(tb_playerquest_record* record,int qid,int accepttime)
{
	void* buf;
	int len = record->qhistory.Get(buf);
	int num = len/sizeof(KDBQuestHistoryDataUnit);
	KDBQuestHistoryDataUnit* pHistory = (KDBQuestHistoryDataUnit*)buf;
	pHistory +=num;
	pHistory->qid = qid;
	pHistory->val = (int)KQ_QuestOver;
	record->qhistory.actualLength = len + sizeof(KDBQuestHistoryDataUnit);
	record->updateMask(tb_playerquest_record::_QHISTORY);
	CancelQuest(record,qid);
	return true;
}

bool CancelQuest(tb_playerquest_record* record,int qid)
{
	int slot = record->getQuestSlot(qid);
	if(slot<0) return false;
	record->qid[slot]=0;
	record->qstate[slot].Set(NULL,0);
	record->updateMask(BIT(slot*2));
	record->updateMask(BIT(slot*2+1));
	return true;
}

void AddExp(tb_player_record* record,int val)
{
	record->exp += val;
	record->updateMask(tb_player_record::_EXP);
}

}