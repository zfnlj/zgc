#include "KPlayerDeck.h"
#include "../StaticTable/KGameStatic.h"
#include "../StaticTable/StaticData.h"

int KHeroDef::GetLev() const 
{
	return KGameStaticMgr::getSingleton().HeroExpToLevel(_exp);
}

int KHeroDef::GetRace()
{
	KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(_cardId);
	if(pST) return pST->GetRace();
	return 0;
}

int KHeroDef::GetSkillLev(int skillIndex) const
{
	int curLev = GetLev()- skillIndex*MAX_HERO_SKILL_LEV;
	return (curLev >MAX_HERO_SKILL_LEV)? MAX_HERO_SKILL_LEV: curLev;
}

void KHeroDef::rndGenerate(int cardId)
{
	_cardId = cardId;
	_id = STATIC_DATA_INC();
	_exp = 0;

	_lucky = rndGenLucky();
	_strong = rndGenStrong();
	if(_strong<0) _strong = 0;
	memset(_skill,0,sizeof(_skill));
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(i+1);
		_skill[i]._skillId = skill->GetId();
	}
}

void KHeroDef::init(KDeckDefStatic* pDef)
{
	_id = 0;
	_lucky = pDef->getHeroLucky();
	_strong = pDef->getHeroStrong();
	_cardId = pDef->getHero();
}

void KHeroDef::Evolute()
{
	int newLucky = rndGenLucky();
	if(newLucky>_lucky){
		_lucky = newLucky;
	}
	int newStrong = rndGenStrong();
	if(newStrong>_strong){
		_strong = newStrong;
	}
}

int KHeroDef::rndGenLucky()
{
	int rndVal = 0;
	int num=0;
	for(int i=0;i<4;i++){
		rndVal  += g_rnd.GetRandom(0,56);
	}
	if(rndVal>200) rndVal=200;
	return (rndVal>100)? (rndVal-100):(100-rndVal);
}

int KHeroDef::rndGenStrong()
{
	float rndVal = 0;
	int num=0;
	for(int i=0;i<5;i++){
		rndVal += g_rnd.GetRandom(0,23);
	}
	if(rndVal>100) rndVal=100;
	float ret = (rndVal>50)? (rndVal-50):(50-rndVal);
	return ret/10.0f +0.5f;
}


int KHeroDef::GetAtkVal()
{
	float val = 0.0f;

	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetHeroSkill(_skill[i]._skillId);
		if(!skill) continue;
		float luckyVal = (_lucky>i*33)?_lucky -i*33:0;
		luckyVal += skill->GetRateVal(GetSkillLev(i));
		if(luckyVal<=0.0f) luckyVal = 0.0f;
		if(luckyVal>100.0f) luckyVal = 100.0f;
		val += skill->GetPower()*luckyVal;
	}
	val = val*10.0f;
	return (int)(val+0.5f);
}

void KPlayerDeck::CreateOnDeckDef(int id)
{
	m_cardList.clear();
	KDeckDefStatic* deck = KGameStaticMgr::getSingleton().GetDeckDef(id);
	deck->GenCardList(m_cardList);
	m_heroDef.rndGenerate(deck->getHero());
	
}
