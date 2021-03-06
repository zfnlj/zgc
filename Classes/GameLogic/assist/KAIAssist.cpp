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
#include "KSkillAssist.h"

KAIAssist* KAIAssist::getInstance(void)
{
	static KAIAssist sKAIAssist;
	return &sKAIAssist;
}


bool compOnAbilityPriority(const KCardInst* lhs, const KCardInst* rhs)
{
	KAbilityStatic* pAbility1 = KGameStaticMgr::getSingleton().GetAbilityOnId(lhs->GetCardId()*10); 
	KAbilityStatic* pAbility2 = KGameStaticMgr::getSingleton().GetAbilityOnId(rhs->GetCardId()*10); 

	return (pAbility1->GetPriority()< pAbility2->GetPriority());
}

float KAIAssist::_getCardValueRate(KCardInst* pCard)
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
float KAIAssist::_calcCardValue(KCardInst* pCard)
{
	if(pCard && pCard->GetHp()>0){
		return pCard->GetAtk()*0.5 + pCard->GetHp()*0.4 + pCard->m_attr.GetBufVal();
	}else{
		return 0.0f;
	}
}

float KAIAssist::_calcTotalAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList& lst)
{
	float totalVal = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		totalVal += _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
	}
	return totalVal;
}

float KAIAssist::_calcTotalCardVal(KCardInstList& lst)
{
	float totalVal = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		totalVal += _calcCardValue(*it);
	}
	return totalVal;
}

float KAIAssist::_calcCardDuelVal(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
{
	int v1,v2;
	KCardInst atk,def;
	atk.clone(pSrc);
	def.clone(pDes);
	float atkV1 = _calcCardValue(&atk);
	float defV1 = _calcCardValue(&def);
	if(!KBattleGod::getSingleton().DoCardDuel(ctrl,&atk,&def,v1,v2)) return -1.0f;
	float atkV2 = _calcCardValue(&atk);
	float defV2 = _calcCardValue(&def);
	float adjustVal = 0.0f;
	if(def.GetAtk()>4){
		adjustVal = ( def.GetAtk()-4)*( def.GetAtk()-4)*0.1f;
	}
	return (atkV2 - atkV1 + defV1 -defV2+def.GetCost()*0.01)+adjustVal;
}

float KAIAssist::_calcAbilityDoVal(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes)
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

KCardInst* KAIAssist::_LestAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& minVal)
{
	if(!lst){
		minVal = 0;
		return NULL;
	}
	KCardInst* pBest = NULL;
	minVal = 0;
	for(KCardInstList::iterator it= lst->begin();it!=lst->end();++it){
		KCardInst* pCur = *it;
		float val = _calcAbilityDoVal(ctrl,pAbility,pSrc,pCur);
		if(val<minVal){
			minVal = val;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* KAIAssist::_MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lstMy,KCardInstList* lst,float& retVal)
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

KCardInst* KAIAssist::_MostAbilityDoValTarget(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInstList* lst,float& maxVal)
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

KCardInst* KAIAssist::_BestAttackTarget(KBattleCtrlBase* ctrl,KCardInst* pAtk,KCardInstList* enemyLst,float maxAtk,float minAtk)
{
	KCardInst* pBest = NULL;
	float val = -1;
	for(KCardInstList::iterator it= enemyLst->begin();it!=enemyLst->end();++it){
		KCardInst* pCur = *it;
		if(pCur->GetAtk()>maxAtk) continue;
		if(pCur->GetAtk()<minAtk) continue;
		float curVal = _calcCardDuelVal(ctrl,pAtk,pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* KAIAssist::_MostValuableTargetNoBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf)
{
	KCardInst* pBest = NULL;
	float val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(pCur->FindBuf(buf)) continue;
		float curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* KAIAssist::_MostValuableTarget(KCardInstList& lst,float maxHp,float minHp)
{
	KCardInst* pBest = NULL;
	float val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(pCur->GetHp()>maxHp) continue;
		if(pCur->GetHp()<minHp) continue;
		float curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

float KAIAssist::_CalcDispelGoodBufVal(KCardInstList& lst)
{
	float val = 0.0f;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		val += pCur->CalcGoodBufVal();
	}
	return val;
}

KCardInst* KAIAssist::_MostValuableBufTarget(KCardInstList& lst)
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

KCardInst* KAIAssist::_MostValuableTargetExistBuf(KCardInstList& lst, KAbilityStatic::Enum_What buf)
{
	KCardInst* pBest = NULL;
	float val = 0;
	for(KCardInstList::iterator it= lst.begin();it!=lst.end();++it){
		KCardInst* pCur = *it;
		if(!pCur->FindBuf(buf)) continue;
		float curVal = _calcCardValue(pCur);
		if(curVal > val){
			val = curVal;
			pBest = pCur;
		}
	}
	return pBest;
}

KCardInst* KAIAssist::AbilityMostValuableTarget(void* ctrl,KCardInst* card,int abilityId,float maxHp,float minHp)
{
	KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(abilityId);
	KCardInstList lst;
	KCardInstList lstMy;
	KSkillAssist::_fillYourAbilityTarget((KBattleCtrlBase*)ctrl,card,NULL,pAbility,&lst);
	KSkillAssist::_fillMyAbilityTarget((KBattleCtrlBase*)ctrl,card,NULL,pAbility,&lstMy);

	KCardInst* pBest =_MostValuableTarget(lst,maxHp,minHp);
	if(pBest) return pBest;
	return _MostValuableTarget(lstMy,maxHp,minHp);
}

float KAIAssist::CalcAbilityDoVal(void* ctrl,int abilityId,KCardInst* pSrc,KCardInst* pDes)
{
	KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(abilityId);
	return _calcAbilityDoVal((KBattleCtrlBase*)ctrl,pAbility,pSrc,pDes);
}

int KAIAssist::_calcTotalActiveAtk(KCardInstList* lst)
{
	int atkVal = 0;
	for(KCardInstList::iterator it= lst->begin();it!=lst->end();++it){
		KCardInst* card = *it;
		if(!card->m_attr.getReady()) continue;
		KAbilityStatic* pNoReady = KSkillAssist::_findStaticAbility(card->GetCardId(),KAbilityStatic::what_noready);
		if(pNoReady) continue;
		atkVal += card->GetAtk();
	}
	return atkVal;
}


int KAIAssist::_calcTotalAtk(KCardInstList* lst)
{
	int atkVal = 0;
	for(KCardInstList::iterator it= lst->begin();it!=lst->end();++it){
		KCardInst* card = *it;
		KAbilityStatic* pNoReady = KSkillAssist::_findStaticAbility(card->GetCardId(),KAbilityStatic::what_noready);
		if(pNoReady) continue;
		atkVal += card->GetAtk();
	}
	return atkVal;
}

KCardInst* KAIAssist::BestAttackTarget(void* ctrl,KCardInst* pAtk,void* lst,float maxAtk,float minAtk)
{
	return _BestAttackTarget((KBattleCtrlBase*)ctrl,pAtk,(KCardInstList*)lst,maxAtk,minAtk);
}
float KAIAssist::CalcHandSetVal(void* ctrl,int myFlag)
{
	KBattleDeck& deck = (myFlag>0)?((KBattleCtrlBase*)ctrl)->GetCurGuy()->GetDeck():((KBattleCtrlBase*)ctrl)->GetDefGuy()->GetDeck();
	KCardInstList* cardSet = deck.QueryCardSet(KCardInst::enum_slot_hand);
	
	float val=0.0f;
	for(KCardInstList::iterator it= cardSet->begin();it!=cardSet->end();++it){
		val += (*it)->GetCost();
	}
	return val;
}

float KAIAssist::CalcFighterSetVal(void* ctrl,int myFlag)
{
	KBattleDeck& deck = (myFlag>0)?((KBattleCtrlBase*)ctrl)->GetCurGuy()->GetDeck():((KBattleCtrlBase*)ctrl)->GetDefGuy()->GetDeck();
	KCardInstList* cardSet = deck.QueryCardSet(KCardInst::enum_slot_fight);

	float val=0.0f;
	for(KCardInstList::iterator it= cardSet->begin();it!=cardSet->end();++it){
		KCardInst* card = *it;
		val += _calcCardValue(card);
	}
	return val;
}

int KAIAssist::CalcFighterNum(void* ctrl,int myFlag)
{
	KBattleDeck& deck = (myFlag>0)?((KBattleCtrlBase*)ctrl)->GetCurGuy()->GetDeck():((KBattleCtrlBase*)ctrl)->GetDefGuy()->GetDeck();
	KCardInstList* cardSet = deck.QueryCardSet(KCardInst::enum_slot_fight);
	return cardSet->size();
}

int KAIAssist::GetCurRes(void* ctrl,int myFlag)
{
	KBattleGuy* pGuy = (myFlag>0)?((KBattleCtrlBase*)ctrl)->GetCurGuy():((KBattleCtrlBase*)ctrl)->GetDefGuy();
	return pGuy->GetCurRes();
}

int KAIAssist::CalcHandNum(void* ctrl,int myFlag)
{
	KBattleDeck& deck = (myFlag>0)?((KBattleCtrlBase*)ctrl)->GetCurGuy()->GetDeck():((KBattleCtrlBase*)ctrl)->GetDefGuy()->GetDeck();
	KCardInstList* cardSet = deck.QueryCardSet(KCardInst::enum_slot_hand);
	return cardSet->size();
}