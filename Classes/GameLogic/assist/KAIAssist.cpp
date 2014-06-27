#include "KAIAssist.h"
#include "../KBattleCtrlBase.h"
#include "../KBattleDeck.h"
#include "../KCardInst.h"
#include "../KBattleGuy.h"
#include "../../StaticTable/KAbilityStatic.h"
#include "../../common/KCommonObj.h"
#include "../KDynamicWorld.h"
#include "../../Inc/KLogicMsg.h"
#include "../KBattleGod.h"

namespace KAIAssist
{

int _calcAttackVal(KCardInst* pAtk,KCardInst* pDef)
{
	int total = 0;
	total += pDef->GetAtkedVal(pAtk->GetAtk())*10;
	total -= pAtk->GetAtkedVal((pAtk->FindRealBuf(KAbilityStatic::what_dist))?0:pDef->GetAtk())*8;
	return total;
}

int _calcCardValue(KCardInst* pCard)
{
	return pCard->GetAtk()*5 + pCard->GetHp()*4 + pCard->GetCost();
}

int _calcAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes)
{
	if(!pDes) return 0;
	KCardInst card;
	card.clone(pDes);
	int v1 = _calcCardValue(&card);
	strCardAbilityResult result;
	KBattleGod::getSingleton().DoCardAbility2Des(ctrl,pAbility,pSrc,&card,&result);
	int v2 = _calcCardValue(&card);
	return v2-v1;
}

KCardInst* _MostHpTarget(KCardInstList& lst)
{
	KCardInst* pBest = NULL;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(!pBest ||
			pBest->GetHp() < pCur->GetHp()){
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* _BestAttackTarget(KCardInst* pAtk,KCardInstList& enemyLst)
{
	KCardInst* pBest = NULL;
	int val = -1;
	for(KCardInstList::iterator it= enemyLst.begin();it!=enemyLst.end();++it){
		KCardInst* pCur = *it;
		int curVal = _calcAttackVal(pAtk,pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}


KCardInst* _MostValuableTarget(KCardInstList& lst,int maxHp,int minHp)
{
	KCardInst* pBest = NULL;
	int val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(pCur->GetHp()>maxHp) continue;
		if(pCur->GetHp()<minHp) continue;
		int curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

}
