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
	if(!m_heroDef) return NULL;
	for(int i=MAX_HERO_SKILL_NUM-1;i>=0;i--){
		int nRand = g_rnd.GetRandom(0,100);
		if(m_skill[i].getLev()==0) continue;
		if(m_skill[i]._skill->GetWhen()!=when) continue;
		if(i==2 && nRand> 60) continue;
		if(i==1 && nRand> 80) continue;
		int skillLucky = m_heroDef->GetLucky()-i*33;
		if(skillLucky>33) skillLucky = 33;
		if(skillLucky<0) skillLucky = 0;
		if((m_skill[i].getRateVal()+skillLucky)> g_rnd.GetRandom(0,100)) return &m_skill[i];
	}
	return NULL;
}

void KHeroSkillMgr::SetHero(KHeroDef* pDef)
{
	if(!pDef) return;
	m_heroDef = pDef;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		addSkill(pDef->_skill[i]._id,pDef->GetSkillLev(i));
	}
}

//void KHeroSkillMgr::Init(KDeckDefStatic* pDeckDef)
//{
//	memset(&m_heroDef,0,sizeof(m_heroDef));
//	memset(m_skill,0,sizeof(m_skill));
//	if(!pDeckDef) return;
//	memcpy(m_skill,pDeckDef->GetHeroSkill(),sizeof(m_skill));
//	m_heroDef._lucky = pDeckDef->getHeroLucky();
//	m_heroDef._strong = pDeckDef->getHeroStrong();
//}
