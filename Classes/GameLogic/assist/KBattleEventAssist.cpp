#include "KBattleEventAssist.h"
#include "../KBattleGod.h"
#include "KSkillAssist.h"

namespace KBattleEvtAssist
{
	void _onBattleEvt(Battle_evt evt,KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
	{
		KAbilityStatic::Enum_When when;
		switch(evt){
		case battle_evt_duel_dead:
			{
				if(pDes->FindBuf(KAbilityStatic::when_dead)) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_dead);
				when = KAbilityStatic::when_soldier_dead;
			}
			break;
		case battle_evt_use_skill:
			when = KAbilityStatic::when_use_skill;
			break;
		case battle_evt_hurted:
			if(pDes->FindBuf(KAbilityStatic::when_damaged)) ctrl->AddCardEvtAbility(pDes,KAbilityStatic::when_damaged);
			when = pDes->IsKindOf(KCardStatic::card_soldier)?KAbilityStatic::when_soldier_hurted:KAbilityStatic::when_hero_hurted;
			break;
		default:
			return;
			break;
		}
		KSkillAssist::_fillCtrlCardEvt(ctrl,pDes,when);
	}

	void _onUseSkillCardEvt(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* card)
	{
		KCardInstList* lst = guy->GetDeck().QueryCardSet(KCardInst::enum_slot_fight);
		for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it)
		{
			KBattleGod::getSingleton().DoCardAbilityOnWhen(ctrl,*it,KAbilityStatic::when_use_skill);
		}
	}
}