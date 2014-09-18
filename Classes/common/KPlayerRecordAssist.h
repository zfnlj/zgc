#ifndef _KPLAYERRECORDASSIST_H
#define _KPLAYERRECORDASSIST_H

#include "CommonLogic/KDBDataStruct.h"
#include "../WorldObject/KPlayer.h"



class KPlayerQuestManager;
class KQuestNew;
struct KHeroDef;
namespace KPlayerRecordAssist
{
	class itemCmp
	{
	public:
		typedef KDBBagItemUnit* ptr;
		int operator () (const ptr& a, const ptr& b) const
		{
			return a->_id - b->_id;
		}
	};
	typedef System::Collections::KDynSortedArray<KDBBagItemUnit*,itemCmp> DBBagItemSortArray;

	bool ClearCardDeck(tb_player_record* record,int index);
	bool updateCardDeck(tb_player_record* record,KIntegerList& lst,int index=99);
	bool syncBagToRecord(KWorldObjAbout::KPlayer*,tb_player_record* record);
	bool syncBagFromRecord(KWorldObjAbout::KPlayer* player,tb_player_record* record);
	bool fillDBBuf(char* des,int size,char* src,int len);
	bool addStoreCard(tb_player_record* record,int id,int num,bool& bNew);
	bool addStoreCard(tb_player_record* record,KIntegerList& lst);
	bool addHero(tb_player_record* record,KHeroDef* hero);
	bool delHero(tb_player_record* record,int heroId);
	bool insertCardDeck(tb_player_record* record,KIntegerList& lst);
	bool setCurDeck(tb_player_record* record,int index);

	//money
	void AddCriVal(tb_player_record* record,int money,int exp,int fateStone=0);
	//quest
	bool syncQuestFromRecord(KPlayerQuestManager*,tb_playerquest_record* record);
	void ClearHistory(tb_playerquest_record* record,int qid);
	bool AddQuest(tb_playerquest_record* record,KQuestNew* pQuest);
	bool QuestOk(tb_playerquest_record* record,int qid);
	bool CancelQuest(tb_playerquest_record* record,int qid);
	void UpdataQuestSession(tb_playerquest_record* record,KQuestNew* pQuest);
	int  RemainDailyQuestNum(tb_playerquest_record* record);
	bool UseDailyAwardSlot(tb_playerquest_record* record);

	//hero
	bool updateHero(tb_player_record* record,KHeroDef* hero);
	void DailyStageLost(tb_player_record* record);
	void DailyStageWin(tb_player_record* record,int dailyStageLev,bool& bStageLevUp);
	
}

#endif // __HELLOWORLD_SCENE_H__
