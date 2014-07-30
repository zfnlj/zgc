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
	_lucky = g_rnd.GetRandom(0,100)+1;
	_strong = g_rnd.GetRandom(0,10)-4;
	if(_strong<0) _strong = 0;
	memset(_skill,0,sizeof(_skill));
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(i+1);
		_skill[i]._skillId = skill->GetId();
	}
}

void KPlayerDeck::CreateOnDeckDef(int id)
{
	m_cardList.clear();
	KDeckDefStatic* deck = KGameStaticMgr::getSingleton().GetDeckDef(id);
	deck->GenCardList(m_cardList);
	m_heroDef.rndGenerate(deck->getHero());
	
}