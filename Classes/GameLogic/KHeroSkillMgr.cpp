#include "KHeroSkillMgr.h"
#include "../common/KCommonObj.h"
#include "KBattleGod.h"

int KHeroSkillMgr::HeroSkill::getRateVal()
{
	return _skill->GetRateVal(_lev);
}

void KHeroSkillMgr::HeroSkill::GenDynAbility(KAbilityStatic& dynAbility)
{
	GetRndAbility()->Clone(dynAbility);
	
	dynAbility.GetVal()._val = _skill->GetRndVal(_lev);

}

void KHeroSkillMgr::onTurnBegin(KBattleCtrlBase* ctrl)
{
	m_bDone = false;
	ActiveSkill(ctrl,KAbilityStatic::when_turn_begin);
}

void KHeroSkillMgr::onTurnEnd(KBattleCtrlBase* ctrl)
{
	ActiveSkill(ctrl,KAbilityStatic::when_turn_end);
}

void KHeroSkillMgr::tmpInit(KBattleGuy* guy)
{
	m_Owner = guy;
	addSkill(1001,3);
	addSkill(1002,3);
	addSkill(1003,3);
	
	//addSkill(1003,2);
}

void KHeroSkillMgr::addSkill(int id,int lev)
{
	for(int i=0;i< MAX_HERO_SKILL_NUM;i++){
		if(m_skill[i].IsEmpty()){
			m_skill[i].init(id,lev);
			break;
		}
	}
}

void KHeroSkillMgr::ActiveSkill(KBattleCtrlBase* ctrl,KAbilityStatic::Enum_When when)
{
	if(m_bDone) return;
	HeroSkill* skill = RndSelectSkill(when);
	if(!skill) return;
	skill->GenDynAbility(m_dynAbility);
	KBattleGod::getSingleton().DoCardAbility(ctrl,&m_dynAbility,m_Owner->GetDeck().GetHero());
	m_bDone = true;
}


KHeroSkillMgr::HeroSkill* KHeroSkillMgr::RndSelectSkill(KAbilityStatic::Enum_When when)
{
	int nRand = g_rnd.GetRandom(0,100);
	int val = 0;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		if(m_skill[i].IsEmpty()) break;
		if(m_skill[i]._skill->GetWhen()!=when) continue;
		val += m_skill[i].getRateVal();
		if(nRand<=val) return &m_skill[i];
	}
	return NULL;
}