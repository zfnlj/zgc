#include "KHeroSkillStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>

#include "KGameStatic.h"
#include "../common/KCommonObj.h"


void HeroSkill::init(int id,int lev){
			_id = id;
			_lev = lev;
			_skill = KGameStaticMgr::getSingleton().GetHeroSkill(id);
			//CCAssert(_skill , "Hero skill is null!");
}

int HeroSkill::getRateVal()
{
	return _skill->GetRateVal(_lev);
}

KAbilityStatic* HeroSkill::GetRndAbility()
{
	return _skill->GetRndAbility();
}

void HeroSkill::GenDynAbility(KAbilityStatic& dynAbility)
{
	GetRndAbility()->Clone(dynAbility);
	dynAbility.GetVal()._val = _skill->GetRndVal(_lev);
}


int KHeroSkillStatic::GetRateVal(int lev)
{ 
	if(lev>MAX_HERO_SKILL_LEV||
		lev<=0.0f) return 0;
	return m_rate[lev-1];
}

int KHeroSkillStatic::GetRndVal(int lev)
{ 
	if(lev>MAX_HERO_SKILL_LEV) return 0;
	return g_rnd.GetRandom(m_val1[lev-1],m_val2[lev-1]+1);
}

KAbilityStatic* KHeroSkillStatic::GetRndAbility()
{
	int count=0;
	for(int i=0;i<MAX_SKILL_ABILITY;i++){
		if(m_ability[i]){
			count++;
		}else{
			break;
		}
	}

	if(count==0) return NULL;
	int rnd = g_rnd.GetRandom(0,count);
	return m_ability[rnd];
}

bool KHeroSkillStatic::init()
{
	memset(m_rate,0,sizeof(m_rate));
	memset(m_val1,0,sizeof(m_val1));
	memset(m_val2,0,sizeof(m_val2));
	memset(m_ability,0,sizeof(m_ability));
	memset(m_race,0,sizeof(m_race));
	m_Id = m_rank = 0;
	return true;
}

KHeroSkillStatic* KHeroSkillStatic::create()
{
	KHeroSkillStatic* pDeck = new KHeroSkillStatic;
	pDeck->init();
	return pDeck;
}

void KHeroSkillStatic::Init(System::File::KTabFile2* fileReader)
{
	char sz[256];
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	fileReader->GetString("Name", "", m_Name, 31);
	
	fileReader->GetString("AbilityId", "", sz, 63);
	SetAbilityArr(sz);

	fileReader->GetInteger("RANK", 0, (int*)&m_rank);
	
	fileReader->GetFloat("Power", 0, (float*)&m_power);

	fileReader->GetString("RATE", "", sz, 63);
	SetArrIntVal(m_rate,MAX_HERO_SKILL_LEV,sz);
	
	fileReader->GetString("VAL1", "", sz, 63);
	SetArrIntVal(m_val1,MAX_HERO_SKILL_LEV,sz);

	fileReader->GetString("VAL2", "", sz, 63);
	SetArrIntVal(m_val2,MAX_HERO_SKILL_LEV,sz);

	fileReader->GetString("WHEN", "", sz, 63);
	m_when = KAbilityStatic::Str2When(sz);

	fileReader->GetString("Race", "", sz, 63);
	SetRaceArr(m_race,sz);

}

void KHeroSkillStatic::SetAbilityArr(const char* str)
{
	if(strlen(str)<2) return;

	char strBuf[1024];
	strcpy(strBuf,str);

	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);
	for(int i=0;i<ns;i++){
		int abilityId = atoi(ss[i]);
		m_ability[i] = KGameStaticMgr::getSingleton().GetAbilityOnId(abilityId);
	}
}

void KHeroSkillStatic::SetRaceArr(int* arr,const char* str)
{
	char strBuf[1024];
	strcpy(strBuf,str);

	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);
	memset(m_race,0,sizeof(m_race));

	for(int i=0;i<ns;i++){
		m_race[i] = KCardStatic::getCardRace(ss[i]);
	}
	
}

void KHeroSkillStatic::SetArrIntVal(int* arr,int size,const char* str)
{
	char strBuf[1024];
	strcpy(strBuf,str);

	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);

	int n = (ns>size)?size:ns;

	for(int i=0;i<n;i++){
		arr[i] = atoi(ss[i]);
	}
}

bool KHeroSkillStatic::IsRaceMatch(int race)
{
	for(int i=0;i<MAX_SKILL_RACE_DEF;i++){
		if(m_race[i]== race) return true;
	}
	return false;
}