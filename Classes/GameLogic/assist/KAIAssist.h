#ifndef _KAIASSIST_H
#define _KAIASSIST_H

#include "../KCardInst.h"
#include "../../StaticTable/KConditionStruct.h"
#include "KBattleEventAssist.h"

class KAbilityStatic;
class KBattleCtrlBase;
class KBattleDeck;
struct strCardAbilityResult;
namespace KAIAssist
{
	int _calcAttackVal(KCardInst* pAtk,KCardInst* pDef);
	KCardInst* _BestAttackTarget(KCardInst* pSrc,KCardInstList& enemyLst);
	KCardInst* _MostValuableTarget(KCardInstList& lst,int maxHp=99,int minHp=-1);
	KCardInst* _MostHpTarget(KCardInstList& lst);
	int _calcCardValue(KCardInst* pCard);
	int _calcAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes);
	KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst,int& maxVal);
	KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lstMy,KCardInstList& lst);
	KCardInst* _LestAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst,int& minVal);
	KCardInst* _MostValuableTargetNoBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf);
	int _calcTotalAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst);
	KCardInst* _MostValuableTargetExistBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf);
	KCardInst* _MostValuableBufTarget(KCardInstList& lst);
}

#endif // __HELLOWORLD_SCENE_H__
