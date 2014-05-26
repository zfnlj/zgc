#include "KPlayerDeck.h"
#include "../StaticTable/KGameStatic.h"

void KHeroDef::rndGenerate(int id)
{
	_heroId = id;
	_exp = 0;
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