#pragma once

#include "../Inc/KTypeDef.h"
#include "../common/KCommonObj.h"

#define MAX_HERO_SKILL_LEV 3
#define MAX_HERO_LEV_INDEX 5
class KDeckDefStatic;
struct KHeroDef{
	struct skillDef{
		int _id;
		int _lev;
		const char* GetName() const ;
		bool IsMaxLev() const{
			return _lev>=MAX_HERO_SKILL_LEV;
		}
	};
	int GetLev() const ;
	skillDef _skill[MAX_HERO_SKILL_NUM];
	int GetSkillLev(int skillIndex) const ;
	int _cardId;
	int _id;
	int _lev;
	int _lucky;  //幸运
	int _strong; //英雄初始血量
	int _resLucky; //回合开始多资源
	int _fate; //回合开始可能多抽牌
	int _dump[5];
	void Generate(int id);
	void Clear(){
		memset(this,0,sizeof(KHeroDef));
	}
	int GetCardId()const { return _cardId;}
	int CalcLev();
	int GetRace();
	int GetLucky() const { return _lucky;}
	int GetFate() const { return _fate;}
	void LevUpFate();
	int GetStrong() const;
	int GetResLucky() const { return _resLucky;}
	int rndGenLevLucky();
	int rndGenLevStrong();
	int GetLevUpStoneNum()const;
	int GetSkillLevUpMoney(int skillIndex);
	bool LevUp();
	void LevZero();
	bool IsLevMax(){ return _lev>=MAX_HERO_LEV_NUM;}
	bool SkillLevUp(int skillIndex);
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

typedef std::list<KHeroDef*> KHeroDefList;