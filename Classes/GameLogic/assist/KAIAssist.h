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
}

#endif // __HELLOWORLD_SCENE_H__
