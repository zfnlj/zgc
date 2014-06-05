#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"
#define MAX_HERO_SKILL_NUM 3
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
	void rndGenerate(int id);
	void Clear(){
		memset(this,0,sizeof(KHeroDef));
	}
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
