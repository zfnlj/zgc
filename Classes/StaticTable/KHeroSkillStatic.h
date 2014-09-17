#ifndef _KHEROSKILLSTATIC_H_
#define _KHEROSKILLSTATIC_H_

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>
#include "KAbilityStatic.h"

#define MAX_PNG_FILE_NAME 36
#define MAX_HERO_SKILL_LEV 3
#define MAX_SKILL_ABILITY 6

#define MAX_SKILL_RACE_DEF 5
class KHeroSkillStatic;

struct HeroSkill{
		int _id;
		int _lev;
		KHeroSkillStatic* _skill;
		HeroSkill():_id(0),_lev(0),_skill(NULL){}
		void init(int id,int lev);
		KAbilityStatic* GetRndAbility();
		int getRateVal();
		int getLev(){ return _lev;}
		bool IsEmpty(){ return _id==0;}
		void GenDynAbility(KAbilityStatic& dynAbility);
};

class KHeroSkillStatic
{
public:
    virtual bool init();
	static KHeroSkillStatic* create();
	void Init(System::File::KTabFile2* fileReader);
	
	int GetRateVal(int lev);
	int GetRndVal(int lev);
	KAbilityStatic* GetRndAbility();
	KAbilityStatic::Enum_When GetWhen(){ return m_when;}
	int GetRank(){ return m_rank;}
	int GetId(){ return m_Id;}
	const char* GetName(){ return m_Name;}
	float GetPower(){ return m_power;}
	void SetRaceArr(int* arr,const char* str);
private:
	void SetAbilityArr(const char* str);
	void SetArrIntVal(int* arr,int size,const char* str);
	int m_Id;
	int m_rank;
	float m_power;
	char m_Name[32];
	KAbilityStatic::Enum_When m_when;
	int m_rate[MAX_HERO_SKILL_LEV];
	int m_val1[MAX_HERO_SKILL_LEV];
	int m_val2[MAX_HERO_SKILL_LEV];
	int m_race[MAX_SKILL_RACE_DEF];
	KAbilityStatic* m_ability[MAX_SKILL_ABILITY];

	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KHeroSkillStatic*> KHeroSkillMap;

#endif // _KHEROSKILLSTATIC_H_
