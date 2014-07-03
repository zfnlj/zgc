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

float _calcAttackVal(KCardInst* pAtk,KCardInst* pDef)
{
	float total = 0;
	total += pDef->GetAtkedVal(pAtk->GetAtk())*_getCardValueRate(pDef);

	total -= pAtk->GetAtkedVal((pAtk->FindRealBuf(KAbilityStatic::what_dist))?0:pDef->GetAtk())*0.5;
	return total;
}

float _getCardValueRate(KCardInst* pCard)
{
	if(pCard->IsKindOf(KCardStatic::card_hero)){
		if(pCard->GetHp()>20){
			return 1.0f;
		}else if(pCard->GetHp()>10){
			return 1.2f;
		}else if(pCard->GetHp()>6){
			return 1.6f;
		}else{
			return 2.0f;
		}
	}else{
		return 1.0f;
	}
}
float _calcCardValue(KCardInst* pCard)
{
	if(pCard && pCard->GetHp()>0){
		return pCard->GetAtk()*0.5 + pCard->GetHp()*0.4 + pCard->m_attr.GetBufVal();
	}else{
		return 0.0f;
	}
}

float _calcTotalAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst)
{
	float totalVal = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		totalVal += _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
	}
	return totalVal;
}

float _calcTotalCardVal(KCardInstList& lst)
{
	float totalVal = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		totalVal += _calcCardValue(*it);
	}
	return totalVal;
}

float _calcAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes)
{
	if(!pDes) return 0;
	KCardInst card;
	card.clone(pDes);
	float v1 = _calcCardValue(&card);
	strCardAbilityResult result;
	KBattleGod::getSingleton().DoCardAbility2Des(ctrl,pAbility,pSrc,&card,&result);
	float v2 = _calcCardValue(&card);
	return (v2-v1)*_getCardValueRate(&card);
}

KCardInst* _LestAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& minVal)
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

KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lstMy,KCardInstList* lst,float& retVal)
{
	float maxVal = 0;
	float minVal = 0;
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

KCardInst* _MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& maxVal)
{
	if(!lst){
		maxVal = 0;
		return NULL;
	}
	KCardInst* pBest = NULL;
	maxVal = 0;
	for(KCardInstList::iterator it= lst->begin();it!=lst->end();++it){
		KCardInst* pCur = *it;
		float val = _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
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
	float val = -1;
	for(KCardInstList::iterator it= enemyLst.begin();it!=enemyLst.end();++it){
		KCardInst* pCur = *it;
		float curVal = _calcAttackVal(pAtk,pCur);
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
	float val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(pCur->FindRealBuf(buf)) continue;
		float curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* _MostValuableTarget(KCardInstList& lst,float maxHp,float minHp)
{
	KCardInst* pBest = NULL;
	float val = 0;
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
	float val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		float curVal = pCur->m_attr.GetBufVal();
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
	float val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(!pCur->FindRealBuf(buf)) continue;
		float curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}
}

