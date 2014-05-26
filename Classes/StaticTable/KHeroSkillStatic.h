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
private:
	void SetAbilityArr(const char* str);
	void SetArrIntVal(int* arr,int size,const char* str);
	int m_Id;
	int m_rank;
	char m_Name[32];
	KAbilityStatic::Enum_When m_when;
	int m_rate[MAX_HERO_SKILL_LEV];
	int m_val1[MAX_HERO_SKILL_LEV];
	int m_val2[MAX_HERO_SKILL_LEV];
	KAbilityStatic* m_ability[MAX_SKILL_ABILITY];

	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KHeroSkillStatic*> KHeroSkillMap;

#endif // _KHEROSKILLSTATIC_H_
