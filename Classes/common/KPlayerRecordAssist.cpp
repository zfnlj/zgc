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
	record->_normalItem.Set(NULL,0);
	KBag::CellMapVec::const_iterator itorTmp = pBag->m_ListItem.begin();
	for(;itorTmp!=pBag->m_ListItem.end();itorTmp++)
	{
		const KCellBag* pCell = (const KCellBag*)(itorTmp->second);
		KDBBagItemUnit dbItem(pCell->GetItem().GetID(),pCell->GetItem().GetStackNumber());
		record->_normalItem.Append(&dbItem,sizeof(dbItem));
	}
	
	return KUserSql::UpdateNormalBag( player->GetName(),record);
}

bool ClearCardDeck(tb_player_record* record,int index)
{
	if(index<0 || index>=MAX_DECK_NUM) return false;
	record->_cardDeck[index].Set(NULL,0);
	record->updateMask(tb_player_record::_CARDDECK0<<index);
	return true;
}

bool updateCardDeck(tb_player_record* record,KIntegerList& lst,int index)
{
	if(lst.size()>31) return false;

	if(index >=MAX_DECK_NUM){
		for(int i=0;i<MAX_DECK_NUM;i++){
			if(record->_cardDeck[i].actualLength==0){
				index = i;
				break;
			}
		}
	}
	if(index>=MAX_DECK_NUM) return false;

	record->_cardDeck[index].Set(NULL,0);
	for(KIntegerList::iterator it=lst.begin();it!=lst.end();++it){
		int id = *it;
		record->_cardDeck[index].Append(&id,sizeof(int));
	}
	record->updateMask(tb_player_record::_CARDDECK0<<index);
	return true;
}

bool delHero(tb_player_record* record,int heroId)
{
	KHeroDefList tmpLst;
	int heroNum = record->_heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)record->_heroData.binData;
	for(int i=0;i<heroNum;i++,pHero++){
		if(pHero->_id!=heroId){
			tmpLst.push_back(pHero);
		}
	}
	record->_heroData.Set(NULL,0);
	for(KHeroDefList::iterator it=tmpLst.begin();it!=tmpLst.end();++it){
		record->_heroData.Append(*it,sizeof(KHeroDef));
	}
	record->updateMask(tb_player_record::_HERODATA);
	return true;
}

bool addHero(tb_player_record* record,KHeroDef* hero)
{
	if(record->_heroData.actualLength>=MAX_HERO_NUM*sizeof(KHeroDef)) return false;

	record->_heroData.Append(hero,sizeof(KHeroDef));
	record->updateMask(tb_player_record::_HERODATA);
	return true;
}

bool setCurDeck(tb_player_record* record,int index)
{
	if(index >=MAX_DECK_NUM) return false;
	if(record->_cardDeck[index].actualLength==0) return false;
	record->_curDeck = index;
	record->updateMask(tb_player_record::_CURDECK);
	return true;
}

bool addStoreCard(tb_player_record* record,KIntegerList& lst)
{
	for(KIntegerList::iterator it=lst.begin();it!=lst.end();++it){
		void* buf=NULL;
		int len = record->_cardStore.Get(buf);
		int id = *it;
		KDBBagItemUnit* dbCard = (KDBBagItemUnit*)buf;
		int n = len/sizeof(KDBBagItemUnit);
		bool bFound = false;
		for(int i=0;i<n;i++){
			if(dbCard->_id==id){
				dbCard->_count ++;
				bFound = true;
				break;
			}
			dbCard++;
		}
		if(!bFound){
			KDBBagItemUnit addCard(id,1);
			record->_cardStore.Append(&addCard,sizeof(addCard));
		}
	}
	record->updateMask(tb_player_record::_CARDSTORE);
	return true;
}

bool addStoreCard(tb_player_record* record,int id,int count,bool& bNew)
{
	bNew = false;
	void* buf=NULL;
	int len = record->_cardStore.Get(buf);
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
		record->_cardStore.Append(&addCard,sizeof(addCard));
	}
	record->updateMask(tb_player_record::_CARDSTORE);
	return true;
}

bool syncBagFromRecord(KWorldObjAbout::KPlayer* player,tb_player_record* record)
{
	void* buf;
	int len = record->_normalItem.Get(buf);
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

int  RemainDailyQuestNum(tb_playerquest_record* record)
{
	void* buf;
	time_t curTime = _GetSystemTimeVal();
	int num = record->_qdaily.Get(buf)/sizeof(time_t);
	int count = MAX_DAILY_AWARD_SLOT - num;
	if(count<0) count=0;

	time_t* dailyTime = (time_t*)buf;
	struct tm *cur_tm = localtime(&curTime);
	int cur_yday = cur_tm->tm_yday;
	for(int i=0;i<num;i++,dailyTime++){

		struct tm *old_tm = localtime(dailyTime);
		if(old_tm->tm_yday!=cur_yday){
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
	int num = record->_qdaily.Get(buf)/sizeof(time_t);
	struct tm *cur_tm = localtime(&curTime);
	int cur_yday = cur_tm->tm_yday;
	if(num<MAX_DAILY_AWARD_SLOT){
		record->_qdaily.Append(&curTime,sizeof(curTime));
		ok=true;
	}else{
		time_t* dailyTime = (time_t*)buf;
		for(int i=0;i<num;i++,dailyTime++){
			struct tm *old_tm = localtime(dailyTime);
			if(old_tm->tm_yday!=cur_yday){
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
		int qid = record->_qid[i];
		KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qid);
		if(!pQuest) continue;
		KQuestNew* pAdd =playerQuestMgr->AddQuest(pQuest);
		void* buf;
		int len = record->_qstate[i].Get(buf);
		int num = len/sizeof(KQuestSessionItem);
		KQuestSessionItem * pSessionItem = (KQuestSessionItem*)buf;
		KClientQuestSession* pSession = (KClientQuestSession*)pAdd->m_pSession;
		pSession->InitSessions(pSessionItem,num);
		pAdd->SyncStatusFromSession();
	}
	void* buf;
	int num = record->_qhistory.Get(buf)/sizeof(KDBQuestHistoryDataUnit);
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
		if(record->_qid[i]==0){
			emptySlot = i;
			break;
		}
	}
	char buf[160];
	if(emptySlot<0) return false;
	record->_qid[emptySlot] = pQuest->m_qid;
	KClientQuestSession* pClientSession = (KClientQuestSession*)pQuest->GetSessionObj();
	int len = pClientSession->QueryData(buf);
	record->_qstate[emptySlot].Set(buf,len);
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
		record->_qid[slot] = 0;
		record->_qstate[slot].Set(NULL,0);
		record->updateMask(BIT(slot*2));
		record->updateMask(BIT(slot*2+1));
	}else{
		record->_qstate[slot].Set(buf,len);
		record->updateMask(BIT(slot*2+1));
	}
}

bool QuestOk(tb_playerquest_record* record,int qid)
{
	KDBQuestHistoryDataUnit history;
	history.qid = qid;
	history.val = (int)KQ_QuestOver;
	history.time_h = _GetSystemTimeVal();
	record->_qhistory.Append(&history,sizeof(KDBQuestHistoryDataUnit));
	record->updateMask(tb_playerquest_record::_QHISTORY);
	CancelQuest(record,qid);
	return true;
}


bool CancelQuest(tb_playerquest_record* record,int qid)
{
	int slot = record->getQuestSlot(qid);
	if(slot<0) return false;
	record->_qid[slot]=0;
	record->_qstate[slot].Set(NULL,0);
	record->updateMask(BIT(slot*2));
	record->updateMask(BIT(slot*2+1));
	return true;
}

void DailyStageLost(tb_player_record* record)
{
	record->_gameData._winDailyStageNum=0;
	record->updateMask(tb_player_record::_CRI);
}

void DailyStageWin(tb_player_record* record,int dailyStageLev,bool& bStageLevUp)
{
	bStageLevUp = false;
	if(record->_gameData.GetDailyStageLev() != dailyStageLev) return;
	record->_gameData._winDailyStageNum++;
	if(record->_gameData._winDailyStageNum>=3 && record->_gameData.GetDailyStageLev()< 19 ){
		record->_gameData._winDailyStageNum = 0;
		record->_gameData._dailyStageLev++;
		bStageLevUp = true;
	}
	record->updateMask(tb_player_record::_CRI);
}

void AddCriVal(tb_player_record* record,int money,int exp,int fateStone)
{
	record->_exp += exp;
	record->_money += money;
	record->_fateStone += fateStone;
	record->updateMask(tb_player_record::_CRI);
}

int GetFateStoneNum(tb_player_record* record)
{
	return record->_fateStone;
}

bool ConsumeFateStone(tb_player_record* record,int num)
{
	if(record->_fateStone<num) return false;
	record->_fateStone -= num;
	record->updateMask(tb_player_record::_CRI);
	return true;
}

bool updateHero(tb_player_record* record,KHeroDef* heroDef)
{
	int heroNum = record->_heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)record->_heroData.binData;
	for(int i=0;i<heroNum;i++,pHero++){
		if(pHero->_id==heroDef->_id){
			memcpy(pHero,heroDef,sizeof(KHeroDef));
			break;
		}
	}
	record->updateMask(tb_player_record::_HERODATA);
	return true;
}

}