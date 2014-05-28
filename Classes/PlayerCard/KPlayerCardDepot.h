#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"
#include "KPlayerDeck.h"
#define MAX_HERO_NUM 10
#define MAX_GUY_CARD_NUM 512

struct tb_player_record;
class KPlayerCardDepot
{
public:
	KPlayerCardDepot(void){}
	~KPlayerCardDepot(void){}
	bool CreateOnDeckDef(int id);
	void init(tb_player_record* record){ m_record = record;}
private:
	tb_player_record* m_record;
};
