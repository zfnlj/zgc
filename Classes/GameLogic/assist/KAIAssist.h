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

	static KCardInst* _BestAttackTarget(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInstList* enemyLst,float maxAtk=99.0f,float minAtk=-1.0f);
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
	static float _calcCardDuelVal(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes);
	static float _getCardValueRate(KCardInst* pCard);
	static int _calcTotalAtk(KCardInstList* lst);
	KCardInst* AbilityMostValuableTarget(void* ctrl,KCardInst* card,int abilityId,float maxHp=99.0f,float minHp=-1.0f);
	float CalcAbilityDoVal(void* ctrl,int abilityId,KCardInst* pSrc,KCardInst* pDes);
	KCardInst* BestAttackTarget(void* ctrl,KCardInst* pAtk,void* lst,float maxAtk=99.0f,float minAtk=-1.0f);
	float CalcFighterSetVal(void* ctrl,int myFlag);
	float CalcHandSetVal(void* ctrl,int myFlag);
	int CalcFighterNum(void* ctrl,int myFlag);
BeginDefLuaClass(KAIAssist)
	DefMemberFunc(AbilityMostValuableTarget)
	DefMemberFunc(CalcAbilityDoVal)
	DefMemberFunc(BestAttackTarget)
	DefMemberFunc(CalcFighterSetVal)
	DefMemberFunc(CalcHandSetVal)
	DefMemberFunc(CalcFighterNum)
EndDef

};

#endif // __HELLOWORLD_SCENE_H__
