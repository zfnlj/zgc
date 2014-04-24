#ifndef _KBATTLEEVENTASSIST_H
#define _KBATTLEEVENTASSIST_H


enum Battle_evt{
	battle_evt_duel_dead,
	battle_evt_use_skill,
	battle_evt_hurted,
};

class KBattleCtrlBase;
class KCardInst;
class KBattleGuy;
namespace KBattleEvtAssist
{
	void _onBattleEvt(Battle_evt evt,KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes);

	void _onUseSkillCardEvt(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* card);
}

#endif // 
