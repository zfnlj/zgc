#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"
#include "CommonLogic/KDBDataStruct.h"

#include "KPlayerDeck.h"
#define MAX_HERO_NUM 10
#define MAX_GUY_CARD_NUM 512

struct tb_player_record;

typedef std::list<KDBBagItemUnit> KItemUnitList;

class KBattleDeck;
namespace KWorldObjAbout
{
	class KPlayer;
}

class KPlayerCardDepot
{
public:
	KPlayerCardDepot(void){}
	~KPlayerCardDepot(void){}
	bool CreateOnDeckDef(int id);
	void init(tb_player_record* record,KWorldObjAbout::KPlayer* player){ m_record = record; m_player=player;}
	bool GetCardDeck(int index,KIntegerList& tmpLst,KHeroDef& hero);
	bool PickCurDeck(int& hero,KIntegerList& tmpLst);
	bool PickDeckHero(int index,KHeroDef& hero);
	bool PickCurHero(KHeroDef& hero);
	bool PickStoreCard(KItemUnitList& tmpLst);
	bool PickAllHero(KHeroDefList& lst);
	int  GetCardNum(int cardId);
	int  GetDeckCardNum(int deck,int cardId);
	int  GetHeroNum();
	bool IsDeckHero(int heroId);
	const KHeroDef* FindHeroOnIndex(int index);
	KHeroDef* FindHero(int id);
	int GetCurDeck();
	bool SaveDeck(int deckId,KIntegerList& tmpLst);
	bool ClearDeck(int deckId);
	void SetCurDeck(int index);
	bool IsDeckReady(int index);
	int CreateHero(int heroId);
	void SaveHero(KHeroDef* hero);
	int BreakHero(int heroId);
private:
	bool FillHeroDef(int heroId,KHeroDef& hero);
	tb_player_record* m_record;
	KWorldObjAbout::KPlayer* m_player;
};
