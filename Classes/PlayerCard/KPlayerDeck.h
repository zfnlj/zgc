#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"

#define MAX_HERO_SKILL_LEV 3
#define MAX_HERO_LEV_INDEX 3
class KDeckDefStatic;
struct KHeroDef{
	struct skillDef{
		int _skillId;
		int _lev;
	};
	int GetLev() const ;
	skillDef _skill[MAX_HERO_SKILL_NUM];
	int GetSkillLev(int skillIndex) const ;
	int _cardId;
	int _id;
	int _lev;
	int _lucky;  //пртк
	int _strong;

	void rndGenerate(int id);
	void Clear(){
		memset(this,0,sizeof(KHeroDef));
	}
	int GetRace();
	int GetLucky() const { return _lucky;}
	int GetStrong() const;
	int rndGenLevLucky();
	int GetLevUpStoneNum();
	int GetSkillLevUpMoney(int skillIndex);
	int LevUp(int stoneNum);
	int SkillLevUp(int skillIndex,int money);
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
