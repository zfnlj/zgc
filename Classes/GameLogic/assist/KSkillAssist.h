#ifndef _KSKILLASSIST_H
#define _KSKILLASSIST_H

#include "../KCardInst.h"

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
}

#endif // __HELLOWORLD_SCENE_H__
