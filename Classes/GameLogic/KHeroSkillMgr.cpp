#include "KHeroSkillMgr.h"
#include "../common/KCommonObj.h"
#include "KBattleGod.h"


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

size_t KHeroSkillMgr::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	for(int i=0;i< MAX_HERO_SKILL_NUM;i++){
		if(!so->WriteWord(m_skill[i]._id)) return 0;
		if(!so->WriteByte(m_skill[i]._lev)) return 0;
	}
	return so->size() - pos;
}

BOOL KHeroSkillMgr::deserialize(KMemoryStream* si)
{
	WORD id;
	BYTE lev;
	for(int i=0;i< MAX_HERO_SKILL_NUM;i++){
		si->ReadWord(id);
		si->ReadByte(lev);
		m_skill[i].init(id,lev);
	}
	return TRUE;
}

void KHeroSkillMgr::addSkill(int id,int lev)
{
	if(lev<1) return;
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
	ctrl->AddDramaElapsed(2.0f);
	KBattleGod::getSingleton().DoCardAbility(ctrl,&m_dynAbility,m_Owner->GetDeck().GetHero());
	m_bDone = true;
}


HeroSkill* KHeroSkillMgr::RndSelectSkill(KAbilityStatic::Enum_When when)
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

void KHeroSkillMgr::SetHero(KHeroDef* pDef)
{
	if(!pDef) return;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		addSkill(pDef->_skill[i]._skillId,pDef->GetSkillLev(i));
	}
}

void KHeroSkillMgr::SetSkill(KDeckDefStatic* pDeckDef)
{
	memset(m_skill,0,sizeof(m_skill));
	if(!pDeckDef) return;
	memcpy(m_skill,pDeckDef->GetHeroSkill(),sizeof(m_skill));
}
