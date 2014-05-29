#include "KPlayerDeck.h"
#include "../StaticTable/KGameStatic.h"
#include "../StaticTable/StaticData.h"

void KHeroDef::rndGenerate(int id)
{
	_heroId = id;
	_deck = -1;
	_Id = STATIC_DATA_INC();
	memset(_skill,0,sizeof(_skill));
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(i+1);
		_skill[i]._skillId = skill->GetId();
	}
}


void KPlayerDeck::CreateOnDeckDef(int id)
{
	int n = sizeof(KHeroDef);
	m_cardList.clear();
	KDeckDefStatic* deck = KGameStaticMgr::getSingleton().GetDeckDef(id);
	deck->GenCardList(m_cardList);
	m_heroDef.rndGenerate(deck->getHero());
	
}