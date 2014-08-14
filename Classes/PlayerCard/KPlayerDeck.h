#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"

#define MAX_HERO_SKILL_LEV 3
#define MAX_HERO_LEV_INDEX 3
class KDeckDefStatic;
struct KHeroDef{
	struct skillDef{
		int _skillId;
		int _exp;
	};
	int GetLev() const ;
	skillDef _skill[MAX_HERO_SKILL_NUM];
	int GetSkillLev(int skillIndex) const ;
	int _cardId;
	int _id;
	int _lev;
	int _strong; //дма╕
	int _lucky;  //пртк

	void rndGenerate(int id);
	void Clear(){
		memset(this,0,sizeof(KHeroDef));
	}
	int GetRace();
	int GetLucky() const { return _lucky;}
	int GetStrong() const {return _strong;}
	int rndGenStrong();
	int rndGenLucky();
	int GetLevUpStoneNum();
	void Evolute();
	int GetAtkVal() const;
	void init(KDeckDefStatic*);
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
