#ifndef __KBATTLEGOD_H__
#define __KBATTLEGOD_H__

#include "System/Singleton.h"
#include "KBattleCtrlBase.h"
#include "../StaticTable/KAbilityStatic.h"


class KBattleGuy;
class KCardInst;
class KAbilityStatic;

class KBattleGod : public Singleton<KBattleGod>
{
public:
	KBattleGod( ){}

	virtual ~KBattleGod(){};
	void init();
	static KBattleGod& getSingleton(void);
	void ProcSlotCard(KCardInst* pSrc,KCardInst* pDes);
	bool DoCardToFightField(KBattleCtrlBase*,KBattleGuy*,KCardInst*,KCardInst* pDes,int pos=-1);
	bool DoCardToSecretField(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pCard);
	void OnTurnBegin();
	bool OnPlayCard(KBattleCtrlBase* ctrl,KBattleCtrlBase::BattleOp*);
	void OnTurnEnd();

	bool DoCardAbility(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes=NULL);
	bool DoCardAbilityOnWhen(KBattleCtrlBase* ctrl,KCardInst* card,KAbilityStatic::Enum_When when);
	void DoCardAbility2Des(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes,strCardAbilityResult* result);
private:
	KBattleGuy* m_TurnPlayer;

	bool ProcHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,int pos);
	bool ProcEquipCard(KCardInst* pSrc,KCardInst* pDes);
	bool ProcCardDuel(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes);
	bool ProcHeroCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes);
    bool ProcSecretCardAbility(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic::Enum_When);
	bool DoUseSkillCard(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pSrc,KCardInst* pDes);
    bool DoUseSecretCard(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pSrc);
	
	void PostCardDuel(KBattleCtrlBase* ctrl,KCardInst* pCard1,KCardInst* pCard2);
	void SendDuelResult(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,int v1,int v2);
	void SendAbilityResult(KBattleCtrlBase* ctrl,strCardAbilityResult& result);
	void AddRes(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility);
	void DrawCard(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility);
	void CopyHand(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility);
	void CopyFight(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility);
	void SummonCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility);
};


#endif // __HELLOWORLD_SCENE_H__

