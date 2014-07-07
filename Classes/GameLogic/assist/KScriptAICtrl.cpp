#include "KScriptAICtrl.h"
#include "../KBattleCtrlBase.h"
#include "../KBattleAI.h"

KScriptAICtrl* KScriptAICtrl::getInstance(void)
{
	static KScriptAICtrl sKScriptAICtrl;
	return &sKScriptAICtrl;
}


float KScriptAICtrl::CalcUseTargetSkillGood(KCardInst* pCard,int abilityId)
{
	KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(abilityId);
	float ret = m_pAI->CalcUseTargetSkillGood(pCard,pAbility,m_Target);
	m_Val += ret;
	return ret;
}