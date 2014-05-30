#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"
#include "KPlayerDeck.h"
#define MAX_HERO_NUM 10
#define MAX_GUY_CARD_NUM 512

struct tb_player_record;
class KBattleDeck;
class KPlayerCardDepot
{
public:
	KPlayerCardDepot(void){}
	~KPlayerCardDepot(void){}
	bool CreateOnDeckDef(int id);
	void init(tb_player_record* record){ m_record = record;}
	bool GetCardDeck(int index,KIntegerList& tmpLst,KHeroDef& hero);
	bool PickCurDeck(int& hero,KIntegerList& tmpLst);
	bool PickCurHero(KHeroDef& hero);
	KHeroDef* FindHero(int id);
	int GetCurDeck();
private:
	bool FillHeroDef(int heroId,KHeroDef& hero);
	tb_player_record* m_record;
};
