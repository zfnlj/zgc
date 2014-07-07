#ifndef _KAIASSIST_H
#define _KAIASSIST_H

#include "../KCardInst.h"
#include "../../StaticTable/KConditionStruct.h"
#include "KBattleEventAssist.h"
#include "Inc/Lua/KLuaWrap.h"

class KAbilityStatic;
class KBattleCtrlBase;
class KBattleDeck;
struct strCardAbilityResult;
class KAIAssist
{
public:
	static KAIAssist* getInstance(void);

	KAIAssist(){ }
	~KAIAssist(){}

	static float _calcAttackVal(KCardInst* pAtk,KCardInst* pDef);
	static KCardInst* _BestAttackTarget(KCardInst* pSrc,KCardInstList& enemyLst);
	static KCardInst* _MostValuableTarget(KCardInstList& lst,float maxHp=99.0f,float minHp=-1.0f);
	static float _calcCardValue(KCardInst* pCard);
	static float _calcAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes);
	static KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& maxVal);
	static KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lstMy,KCardInstList* lst,float& maxVal);
	static KCardInst* _LestAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& minVal);
	static KCardInst* _MostValuableTargetNoBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf);
	static float _calcTotalAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst);
	static KCardInst* _MostValuableTargetExistBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf);
	static KCardInst* _MostValuableBufTarget(KCardInstList& lst);
	static float _calcTotalCardVal(KCardInstList& lst);
	static float _getCardValueRate(KCardInst* pCard);

	static KCardInst* _AbilityMostValuableTarget(KBattleCtrlBase* ctrl,KCardInst* card,int abilityId,float maxHp=99.0f,float minHp=-1.0f);
BeginDefLuaClass(KAIAssist)
	DefMemberFunc(_getCardValueRate)
	//DefMemberFunc(GetMyHero);
EndDef

};

#endif // __HELLOWORLD_SCENE_H__
