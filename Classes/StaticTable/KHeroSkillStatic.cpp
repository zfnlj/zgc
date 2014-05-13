#include "KHeroSkillStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>
#include "KAbilityStatic.h"
#include "KGameStatic.h"

int KHeroSkillStatic::GetRndVal(int lev)
{ 
	if(lev>MAX_HERO_SKILL_LEV) return 0;
	return m_rate[lev-1];
}
bool KHeroSkillStatic::init()
{
	memset(m_rate,0,sizeof(m_rate));
	memset(m_val1,0,sizeof(m_val1));
	memset(m_val2,0,sizeof(m_val2));
	memset(m_what,0,sizeof(m_what));
	m_Id = m_abilityId = m_rank = 0;
	m_pAbility = NULL;
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
	char sz[64];
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	fileReader->GetString("Name", "", m_Name, 31);
	
	fileReader->GetInteger("AbilityId", 0, (int*)&m_abilityId);
	fileReader->GetInteger("RANK", 0, (int*)&m_rank);
	
	fileReader->GetString("RATE", "", sz, 63);
	SetArrIntVal(m_rate,MAX_HERO_SKILL_LEV,sz);
	
	fileReader->GetString("VAL1", "", sz, 63);
	SetArrIntVal(m_val1,MAX_HERO_SKILL_LEV,sz);

	fileReader->GetString("VAL2", "", sz, 63);
	SetArrIntVal(m_val2,MAX_HERO_SKILL_LEV,sz);

	fileReader->GetString("WHAT", "", sz, 63);
	SetWhatArr(sz);
}

void KHeroSkillStatic::SetWhatArr(const char* str)
{
	if(strlen(str)<2) return;

	char strBuf[1024];
	strcpy(strBuf,str);

	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);
	for(int i=0;i<ns;i++){
		m_what[i] =  KAbilityStatic::Str2What(ss[i]);
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

KAbilityStatic* KHeroSkillStatic::GetAbility()
{
	if(!m_pAbility){
		m_pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(m_abilityId);
	}
	return m_pAbility;
}