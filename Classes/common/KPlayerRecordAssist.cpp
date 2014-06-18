#include "KPlayerRecordAssist.h"
#include "../sqlite/KUserSql.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "../Quest/KQuestManager.h"
#include "../Quest/KPlayerQuestManager.h"
#include "../Quest/KClientQuestSession.h"
#include "../Quest/KQuestNew.h"
#include "../PlayerCard/KPlayerDeck.h"
#include "KCommonObj.h"

#define  SECOND_OF_DAY 24*60*60
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

bool ClearCardDeck(tb_player_record* record,int index)
{
	if(index<0 || index>=MAX_DECK_NUM) return false;
	record->cardDeck[index].Set(NULL,0);
	record->updateMask(tb_player_record::_CARDDECK0<<index);
	return true;
}

bool updateCardDeck(tb_player_record* record,KIntegerList& lst,int index)
{
	if(lst.size()>31) return false;

	if(index >=MAX_DECK_NUM){
		for(int i=0;i<MAX_DECK_NUM;i++){
			if(record->cardDeck[i].actualLength==0){
				index = i;
				break;
			}
		}
	}
	if(index>=MAX_DECK_NUM) return false;

	record->cardDeck[index].Set(NULL,0);
	for(KIntegerList::iterator it=lst.begin();it!=lst.end();++it){
		int id = *it;
		record->cardDeck[index].Append(&id,sizeof(int));
	}
	record->updateMask(tb_player_record::_CARDDECK0<<index);
	return true;
}

bool updateHeroExp(tb_player_record* record,int id,int exp)
{
	int heroNum = record->heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)record->heroData.binData;
	for(int i=0;i<heroNum;i++,pHero++){
		if(pHero->_id==id){
			pHero->_exp =exp;
			break;
		}
	}
	record->updateMask(tb_player_record::_HERODATA);
	return true;
}
bool addHero(tb_player_record* record,KHeroDef* hero)
{
	if(record->heroData.actualLength>=MAX_HERO_NUM*sizeof(KHeroDef)) return false;

	record->heroData.Append(hero,sizeof(KHeroDef));
	record->updateMask(tb_player_record::_HERODATA);
	return true;
}

bool setCurDeck(tb_player_record* record,int index)
{
	if(index >=MAX_DECK_NUM) return false;
	if(record->cardDeck[index].actualLength==0) return false;
	record->curDeck = index;
	record->updateMask(tb_player_record::_CURDECK);
	return true;
}

bool addStoreCard(tb_player_record* record,int id,int count,bool& bNew)
{
	bNew = false;
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
		bNew = true;
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

int  getDailyAwardNum(tb_playerquest_record* record)
{
	void* buf;
	time_t curTime = _GetSystemTimeVal();
	int num = record->qdaily.Get(buf)/sizeof(time_t);
	int count = MAX_DAILY_AWARD_SLOT - num;
	if(count<0) count=0;

	time_t* dailyTime = (time_t*)buf;
	for(int i=0;i<num;i++,dailyTime++){
		if( (curTime - *dailyTime)>SECOND_OF_DAY){
			count++;
		}
	}
	return count;
}

bool UseDailyAwardSlot(tb_playerquest_record* record)
{
	bool ok=false;
	time_t curTime = _GetSystemTimeVal();
	void* buf;
	int num = record->qdaily.Get(buf)/sizeof(time_t);
	if(num<MAX_DAILY_AWARD_SLOT){
		record->qdaily.Append(&curTime,sizeof(curTime));
		ok=true;
	}else{
		time_t* dailyTime = (time_t*)buf;
		for(int i=0;i<num;i++,dailyTime++){
			if( (curTime - *dailyTime)>SECOND_OF_DAY){
				*dailyTime = curTime;
				ok = true;
				break;
			}
		}
		
	}
	if(ok) record->updateMask(tb_playerquest_record::_QDAILY);
	return ok;
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
	KDBQuestHistoryDataUnit* pHistory = (KDBQuestHistoryDataUnit*)buf;
	for(int i=0;i<num;i++,pHistory++){
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
	if(slot<0){
		return;
	}
	char buf[160];
	KClientQuestSession* pClientSession = (KClientQuestSession*)pQuest->GetSessionObj();
	int len = pClientSession->QueryData(buf);
	if(pQuest->GetQuestStatus()==KQ_QuestOver){
		record->qid[slot] = 0;
		record->qstate[slot].Set(NULL,0);
		record->updateMask(BIT(slot*2));
		record->updateMask(BIT(slot*2+1));
	}else{
		record->qstate[slot].Set(buf,len);
		record->updateMask(BIT(slot*2+1));
	}
}

bool QuestOk(tb_playerquest_record* record,int qid)
{
	KDBQuestHistoryDataUnit history;
	history.qid = qid;
	history.val = (int)KQ_QuestOver;
	history.time_h = _GetSystemTimeVal();
	record->qhistory.Append(&history,sizeof(KDBQuestHistoryDataUnit));
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

void AddExp(tb_player_record* record,int val,int power)
{
	record->exp += val;
	record->power += power;
	record->updateMask(tb_player_record::_CRI);
}

}