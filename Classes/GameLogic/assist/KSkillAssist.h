#ifndef _KSKILLASSIST_H
#define _KSKILLASSIST_H

#include "../KCardInst.h"

class KAbilityStatic;
class KBattleCtrlBase;
namespace KSkillAssist
{

	void _fillAbilityTarget(KBattleCtrlBase*,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lst,bool bMy=false /*只取我方的*/);
	void _rndFillProc(KAbilityStatic* pAbility,KCardInstList* lst);
	bool _checkSecretAbility(KCardInst* pSrc,KCardInst* pDes,KCardInst* pSecret,KAbilityStatic::Enum_When);
	void _removeBuf(KCardAbilityList& list,int id);
	KAbilityStatic* _findBuf(KCardAbilityList& list,KAbilityStatic::Enum_What what);
}

#endif // __HELLOWORLD_SCENE_H__
