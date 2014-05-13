#ifndef _KHEROSKILLSTATIC_H_
#define _KHEROSKILLSTATIC_H_

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>

#define MAX_PNG_FILE_NAME 36
#define MAX_HERO_SKILL_LEV 3
#define MAX_SKILL_WHAT 6

class KAbilityStatic;
class KHeroSkillStatic
{
public:
    virtual bool init();
	static KHeroSkillStatic* create();
	void Init(System::File::KTabFile2* fileReader);
	
	int GetRndVal(int lev);
	KAbilityStatic* GetAbility();
private:
	void SetWhatArr(const char* str);
	void SetArrIntVal(int* arr,int size,const char* str);
	int m_Id;
	int  m_abilityId;
	int m_rank;
	char m_Name[32];
	int m_rate[MAX_HERO_SKILL_LEV];
	int m_val1[MAX_HERO_SKILL_LEV];
	int m_val2[MAX_HERO_SKILL_LEV];
	int m_what[MAX_SKILL_WHAT];
	KAbilityStatic* m_pAbility;
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KHeroSkillStatic*> KHeroSkillMap;

#endif // _KHEROSKILLSTATIC_H_
