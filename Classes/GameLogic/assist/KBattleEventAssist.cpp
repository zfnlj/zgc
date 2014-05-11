#include "KBattleEventAssist.h"
#include "../KBattleGod.h"
#include "KSkillAssist.h"

namespace KBattleEvtAssist
{
	void _onBattleEvt(Battle_evt evt,KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
	{
		KAbilityStatic::Enum_When when;
		switch(evt){
		case battle_evt_soldier_dead:
			{
				if(pDes->FindBuf(KAbilityStatic::when_dead)) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_dead);
				when = KAbilityStatic::when_soldier_dead;
			}
			break;
		case battle_evt_use_skill:
			when = KAbilityStatic::when_use_skill;
			break;
		case battle_evt_hurted:
			if(pDes->FindBuf(KAbilityStatic::when_damaged)&& !pDes->IsDead()) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_damaged);
			when = pDes->IsKindOf(KCardStatic::card_soldier)?KAbilityStatic::when_soldier_hurted:KAbilityStatic::when_hero_hurted;
			break;
		case battle_evt_healed:
			if(pDes->FindBuf(KAbilityStatic::when_healed)) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_healed);
			when = pDes->IsKindOf(KCardStatic::card_soldier)?KAbilityStatic::when_soldier_healed:KAbilityStatic::when_hero_healed;
			break;
		case battle_evt_use_card:
			when = KAbilityStatic::when_use_card;
			break;
		default:
			return;
			break;
		}
		KSkillAssist::_fillCtrlCardEvt(ctrl,(pDes)?pDes:pSrc,when);
	}
}