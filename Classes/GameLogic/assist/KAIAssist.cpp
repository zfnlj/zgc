#include "KAIAssist.h"
#include "../KBattleCtrlBase.h"
#include "../KBattleDeck.h"
#include "../KCardInst.h"
#include "../KBattleGuy.h"
#include "../../StaticTable/KAbilityStatic.h"
#include "../../common/KCommonObj.h"
#include "../KDynamicWorld.h"
#include "../../Inc/KLogicMsg.h"

namespace KAIAssist
{

int _calcAttackVal(KCardInst* pAtk,KCardInst* pDef)
{
	int total = 0;
	total += pDef->GetAtkedVal(pAtk->GetAtk());
	total -= pAtk->GetAtkedVal((pAtk->FindRealBuf(KAbilityStatic::what_dist))?0:pDef->GetAtk());
	return total;
}

KCardInst* _BestAttackTarget(KCardInst* pAtk,KCardInstList& enemyLst)
{
	KCardInst* pBest = NULL;
	int val = 0;
	for(KCardInstList::iterator it= enemyLst.begin();it!=enemyLst.end();++it){
		KCardInst* pCur = *it;
		int curVal = _calcAttackVal(pAtk,pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return NULL;
}

}
