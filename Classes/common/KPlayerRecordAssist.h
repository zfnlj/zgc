#ifndef _KPLAYERRECORDASSIST_H
#define _KPLAYERRECORDASSIST_H

#include "CommonLogic/KDBDataStruct.h"
#include "../WorldObject/KPlayer.h"



class KPlayerQuestManager;
class KQuestNew;
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
	bool addStoreCard(tb_player_record* record,int id,int num);
	bool addHero(tb_player_record* record,KHeroDef* hero);
	bool insertCardDeck(tb_player_record* record,KIntegerList& lst);
	bool setCurDeck(tb_player_record* record,int index);
	//hero
	bool updateHeroExp(tb_player_record* record,int id,int exp);

	//money
	void AddExp(tb_player_record* record,int val,int power);
	//quest
	bool syncQuestFromRecord(KPlayerQuestManager*,tb_playerquest_record* record);
	void ClearHistory(tb_playerquest_record* record,int qid);
	bool AddQuest(tb_playerquest_record* record,KQuestNew* pQuest);
	bool QuestOk(tb_playerquest_record* record,int qid,int accepttime);
	bool CancelQuest(tb_playerquest_record* record,int qid);
	void UpdataQuestSession(tb_playerquest_record* record,KQuestNew* pQuest);
}

#endif // __HELLOWORLD_SCENE_H__
