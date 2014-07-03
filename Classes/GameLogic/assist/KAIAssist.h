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
	float _calcAttackVal(KCardInst* pAtk,KCardInst* pDef);
	KCardInst* _BestAttackTarget(KCardInst* pSrc,KCardInstList& enemyLst);
	KCardInst* _MostValuableTarget(KCardInstList& lst,float maxHp=99.0f,float minHp=-1.0f);
	float _calcCardValue(KCardInst* pCard);
	float _calcAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes);
	KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& maxVal);
	KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lstMy,KCardInstList* lst,float& maxVal);
	KCardInst* _LestAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& minVal);
	KCardInst* _MostValuableTargetNoBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf);
	float _calcTotalAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst);
	KCardInst* _MostValuableTargetExistBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf);
	KCardInst* _MostValuableBufTarget(KCardInstList& lst);
	float _calcTotalCardVal(KCardInstList& lst);
	void _sortOnAbilityPriority(KCardInstList& lst);
	float _getCardValueRate(KCardInst* pCard);
}

#endif // __HELLOWORLD_SCENE_H__
