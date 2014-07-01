#ifndef _KBATTLEEVENTASSIST_H
#define _KBATTLEEVENTASSIST_H

#include "../../StaticTable/KAbilityStatic.h"
enum Battle_evt{
	battle_evt_soldier_dead,
	battle_evt_soldier_enter,
	battle_evt_use_card,
	battle_evt_use_skill,
	battle_evt_hurted,
	battle_evt_healed,
};

class KBattleCtrlBase;
class KCardInst;
class KBattleGuy;

struct strDoCardWhenAbility{
	KCardInst* _card;
	KAbilityStatic::Enum_When _when;
	strDoCardWhenAbility(KCardInst* card,KAbilityStatic::Enum_When when):_card(card),_when(when){}
};

typedef std::list<strDoCardWhenAbility> KDoCardWhenAbilityList;

namespace KBattleEvtAssist
{
	void _onBattleEvt(Battle_evt evt,KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes);

}

#endif // 
