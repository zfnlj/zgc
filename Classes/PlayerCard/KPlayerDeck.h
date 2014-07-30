#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"

#define MAX_HERO_SKILL_LEV 3

struct KHeroDef{
	struct skillDef{
		int _skillId;
	};
	int GetLev() const ;
	skillDef _skill[MAX_HERO_SKILL_NUM];
	int GetSkillLev(int skillIndex) const ;
	int _cardId;
	int _id;
	int _exp;
	int _strong; //дма╕
	int _lucky;  //пртк

	void rndGenerate(int id);
	void Clear(){
		memset(this,0,sizeof(KHeroDef));
	}
	int GetRace();
	int GetRank();
	int GetLucky(){ return _lucky;}
	int GetStrong(){return _strong;}
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
