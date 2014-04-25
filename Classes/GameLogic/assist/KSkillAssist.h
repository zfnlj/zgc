#ifndef _KSKILLASSIST_H
#define _KSKILLASSIST_H

#include "../KCardInst.h"
#include "../StaticTable/KConditionStruct.h"
#include "KBattleEventAssist.h"
class KAbilityStatic;
class KBattleCtrlBase;
struct strCardAbilityResult;
namespace KSkillAssist
{
	void _fillAbilityTarget(KBattleCtrlBase*,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lstMy,KCardInstList* lstYour);
	void _fillAbilityTarget(KBattleCtrlBase*,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lst,bool bMy=false /*只取我方的*/);
	void _rndFillProc(KAbilityStatic* pAbility,KCardInstList* lst);
	bool _checkSecretAbility(KCardInst* pSrc,KCardInst* pDes,KCardInst* pSecret,KAbilityStatic::Enum_When);
	void _removeBuf(KCardAbilityList& list,int id);
	KAbilityStatic* _findBuf(KCardAbilityList& list,KAbilityStatic::Enum_What what);
	KCardInst* _findActiveSecret(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic::Enum_When when );
	bool _doSecretAbility(KBattleCtrlBase* ctrl,KCardInst* pSecret,KCardInst** pSrc,KCardInst** pDes);
	int _summonCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInst* pActor=NULL);
	void _copyHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility);
	void _copyFightSoldier(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility);
	void _sendAbilityResult(KBattleCtrlBase* ctrl,strCardAbilityResult& result);
	bool _IsMatch(KConditionDef& con,KCardInst* card);
	int _calcAbilityVal(KBattleGuy* guy,KAbilityStatic* pAbility);
	void _fillCtrlCardEvt(KBattleCtrlBase* ctrl,KCardInst* pCard,KAbilityStatic::Enum_When when);
	KAbilityStatic* _findStaticAbility(int cardId,KAbilityStatic::Enum_When when);
}

#endif // __HELLOWORLD_SCENE_H__
