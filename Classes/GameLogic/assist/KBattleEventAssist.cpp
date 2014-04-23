#include "KBattleEventAssist.h"
#include "../KBattleGod.h"

namespace KBattleEvtAssist
{
	void _onBattleEvt(Battle_evt evt,KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
	{
		switch(evt){
		case battle_evt_duel_dead:
			{
				KBattleGod::getSingleton().DoCardAbilityOnWhen(ctrl,pDes,KAbilityStatic::when_dead);
			}
			break;
		case battle_evt_use_skill:
			{
				_onUseSkillCardEvt(ctrl,pSrc->GetOwner(),pSrc);
			}
			break;
		case battle_evt_soldier_hurted:
			{

			}
			break;
		}
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