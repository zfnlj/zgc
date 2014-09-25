#include "KBattleEventAssist.h"
#include "../KBattleGod.h"
#include "KSkillAssist.h"

namespace KBattleEvtAssist
{
	void _onBattleEvt(Battle_evt evt,KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
	{
		if(ctrl->GetBattleState()==KBattleCtrlBase::battle_game_end) return;
		KAbilityStatic::Enum_When when;
		switch(evt){
		case battle_evt_soldier_dead:
			{
				if(pDes->FindBufAbility(KAbilityStatic::when_dead)) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_dead);
				KSkillAssist::_fillCtrlCardEvt(ctrl,pDes,KAbilityStatic::when_soldier_dead);
			}
			break;
		case battle_evt_soldier_enter:
			{
				KSkillAssist::_fillCtrlCardEvt(ctrl,pDes,KAbilityStatic::when_soldier_enter);
			}
			break;
		case battle_evt_use_skill:
			KSkillAssist::_fillCtrlCardEvt(ctrl,pSrc,KAbilityStatic::when_use_skill);
			break;
		case battle_evt_hurted:
			if(pDes->FindBufAbility(KAbilityStatic::when_damaged)&& !pDes->IsDead()) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_damaged);
			when = pDes->IsKindOf(KCardStatic::card_soldier)?KAbilityStatic::when_soldier_hurted:KAbilityStatic::when_hero_hurted;
			KSkillAssist::_fillCtrlCardEvt(ctrl,pDes,when);
			break;
		case battle_evt_healed:
			if(pDes->FindBufAbility(KAbilityStatic::when_healed)) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_healed);
			when = pDes->IsKindOf(KCardStatic::card_soldier)?KAbilityStatic::when_soldier_healed:KAbilityStatic::when_hero_healed;
			KSkillAssist::_fillCtrlCardEvt(ctrl,pDes,when);
			break;
		case battle_evt_use_card:
			KSkillAssist::_fillCtrlCardEvt(ctrl,pSrc,KAbilityStatic::when_use_card,pSrc);
			break;
		default:
			return;
			break;
		}
		
	}
}