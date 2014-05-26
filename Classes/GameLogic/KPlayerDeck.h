#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"
#define MAX_HERO_SKILL_NUM 3
struct KHeroDef{
	struct skillDef{
		int _skillId;
		int _lev;
	};
	DWORD _exp;
	skillDef _skill[MAX_HERO_SKILL_NUM];
	int _heroId;
	void rndGenerate(int id);
};

class KPlayerDeck
{
public:
	KPlayerDeck(void){}
	~KPlayerDeck(void){}
	void CreateOnDeckDef(int id);
private:
	KHeroDef m_heroDef;
	KIntegerList m_cardList;
};
