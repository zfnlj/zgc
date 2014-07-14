#ifndef _KSKILLASSIST_H
#define _KSKILLASSIST_H

#include "../KCardInst.h"
#include "../../StaticTable/KConditionStruct.h"
#include "KBattleEventAssist.h"
class KAbilityStatic;
class KBattleCtrlBase;
class KBattleDeck;
struct strCardAbilityResult;
namespace KSkillAssist
{
	void _fillAllAbilityTarget(KBattleCtrlBase*,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lstMy,KCardInstList* lstYour);
	void _fillAllAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic* pAbility,KCardInstList* lst);
	void _fillMyAbilityTarget(KBattleCtrlBase*,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic* pAbility,KCardInstList* lst);
	void _fillYourAbilityTarget(KBattleCtrlBase*,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic* pAbility,KCardInstList* lst);
	void _rndFillProc(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lst);
	bool _checkSecretAbility(KCardInst* pSrc,KCardInst* pDes,KCardInst* pSecret,KAbilityStatic::Enum_When);
	void _removeBuf(KCardAbilityList& list,int id);
	KAbilityStatic* _findBuf(KCardAbilityList& list,KAbilityStatic::Enum_What what);
	KCardInst* _findActiveSecret(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic::Enum_When when );
	bool _doSecretAbility(KBattleCtrlBase* ctrl,KCardInst* pSecret,KCardInst** pSrc,KCardInst** pDes);
	int _summonCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor);
	void _copyHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor);
	void _clearHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor);
	void _copyFightSoldier(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor);
	void _sendAbilityResult(KBattleCtrlBase* ctrl,strCardAbilityResult& result);
	bool _IsMatch(KConditionDef& con,KCardInst* card);
	int _calcValDef(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst*,KValDef& valDef);
	void _fillCtrlCardEvt(KBattleCtrlBase* ctrl,KCardInst* pCard,KAbilityStatic::Enum_When when);
	KAbilityStatic* _findStaticAbility(int cardId,KAbilityStatic::Enum_When when);
	KAbilityStatic* _findStaticAbility(int cardId,KAbilityStatic::Enum_What what);
	void _abilityGenCard(KBattleCtrlBase* ctrl,KBattleDeck* deck,KCardInst* pSrc,KAbilityStatic* pAbility,int actor);
	void _filterAbility(KCardAbilityList* srcLst,KCardAbilityList* desLst,KAbilityStatic::Enum_When when);
	bool _needEnterFightTarget(int cardId);
	void _sendSkillInterrupt(KBattleCtrlBase* ctrl,KCardInst* card);
}

#endif // __HELLOWORLD_SCENE_H__
