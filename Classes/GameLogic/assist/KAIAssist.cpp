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

bool compOnAbilityPriority(const KCardInst* lhs, const KCardInst* rhs)
{
	KAbilityStatic* pAbility1 = KGameStaticMgr::getSingleton().GetAbilityOnId(lhs->GetCardId()*10); 
	KAbilityStatic* pAbility2 = KGameStaticMgr::getSingleton().GetAbilityOnId(rhs->GetCardId()*10); 

	return (pAbility1->GetPriority()< pAbility2->GetPriority());
}

void _sortOnAbilityPriority(KCardInstList& lst)
{
}

int _calcAttackVal(KCardInst* pAtk,KCardInst* pDef)
{
	int total = 0;
	total += pDef->GetAtkedVal(pAtk->GetAtk())*10;
	if(pDef->IsKindOf(KCardStatic::card_hero)&& pDef->GetHp()<10){
		total += (10-pDef->GetHp())*5;
	}
	total -= pAtk->GetAtkedVal((pAtk->FindRealBuf(KAbilityStatic::what_dist))?0:pDef->GetAtk())*8;
	return total;
}

int _calcCardValue(KCardInst* pCard)
{
	if(pCard->IsKindOf(KCardStatic::card_hero)){
		if(pCard->GetHp()<=0){
			return -9999;
		}else if(pCard->GetHp()<10){
			return pCard->GetHp()*pCard->GetHp();
		}else{
			return pCard->GetHp()*10;
		}
	}else{
		if(pCard->GetHp()>0){
			return pCard->GetAtk()*5 + pCard->GetHp()*4 + pCard->m_attr.GetBufVal()*5;
		}else{
			return 0;
		}
	}
	
}

int _calcTotalAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst)
{
	int totalVal = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		totalVal += _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
	}
	return totalVal;
}

int _calcTotalCardVal(KCardInstList& lst)
{
	int totalVal = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		totalVal += _calcCardValue(*it);
	}
	return totalVal;
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

KCardInst* _LestAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,int& minVal)
{
	if(!lst){
		minVal = 0;
		return NULL;
	}
	KCardInst* pBest = NULL;
	minVal = 0;
	for(KCardInstList::iterator it= lst->begin();it!=lst->end();++it){
		KCardInst* pCur = *it;
		int val = _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
		if(val<minVal){
			minVal = val;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lstMy,KCardInstList* lst,int& retVal)
{
	int maxVal = 0;
	int minVal = 0;
	KCardInst* pBest1 = _MostAbilityDoValTarget(ctrl,pAbility,pSrc,lstMy,maxVal);
	KCardInst* pBest2 = _LestAbilityDoValTarget(ctrl,pAbility,pSrc,lst,minVal);
	if(pBest1&& pBest2){
		if(minVal<0) minVal = -minVal;
		if(maxVal >minVal){
			retVal = maxVal;
			return pBest1;
		}else{
			retVal = minVal;
			return pBest2;
		}
	}else{
		if(pBest1){
			retVal = maxVal;
			return pBest1;
		}
		if(pBest2){
			if(minVal<0) minVal = -minVal;
			retVal = minVal;
			return pBest2;
		}
	}
	return NULL;
}

KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,int& maxVal)
{
	if(!lst){
		maxVal = 0;
		return NULL;
	}
	KCardInst* pBest = NULL;
	maxVal = 0;
	for(KCardInstList::iterator it= lst->begin();it!=lst->end();++it){
		KCardInst* pCur = *it;
		int val = _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
		if(val>maxVal){
			maxVal = val;
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

KCardInst* _MostValuableTargetNoBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf)
{
	KCardInst* pBest = NULL;
	int val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(pCur->FindRealBuf(buf)) continue;
		int curVal = _calcCardValue(pCur);
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

KCardInst* _MostValuableBufTarget(KCardInstList& lst)
{
	KCardInst* pBest = NULL;
	int val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		int curVal = pCur->m_attr.GetBufVal();
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* _MostValuableTargetExistBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf)
{
	KCardInst* pBest = NULL;
	int val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(!pCur->FindRealBuf(buf)) continue;
		int curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}
}

