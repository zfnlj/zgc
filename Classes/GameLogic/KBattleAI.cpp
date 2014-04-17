#include "KBattleAI.h"
#include "KBattleCtrlBase.h"
#include "KBattleGod.h"
#include "KDynamicWorld.h"
#include "../StaticTable/KAbilityStatic.h"
#include "assist/KSkillAssist.h"

KBattleAI::KBattleAI()
{

}
KBattleAI* KBattleAI::create(void)
{
	KBattleAI* pGuy = KBattleAI::Alloc();
	pGuy->init();
	return pGuy;
}

bool KBattleAI::init(void)
{
	m_thinkElapsed = 0;
	KBattleGuy::init();
	return true;
}

void KBattleAI::SelectHandCard()
{
	if(!m_bSelectHandCard) return;
	DoSelectBeginCard(NULL);
}

void KBattleAI::onBattleInit(bool bFirst)
{
	KBattleGuy::onBattleInit(bFirst,0);
}
void KBattleAI::onPlayCard(float dt,bool bOK)
{
	ThinkToPlay(dt);
	KBattleGuy::onPlayCard(dt,bOK);
}


void KBattleAI::ThinkToPlay(float dt)
{
	m_thinkElapsed += dt;
	if(m_thinkElapsed<5)
		return;
	m_thinkElapsed = 0;
	//if(UseSkillCard())
	//	return;
	if(HandCardToField())
		return;
	//if(UseHeroSkill())
	//	return;
	
    if(UseSecretCard())
        return;
	if(SoldierToAttack())
		return;

	SetPlayTimeOut(); //AI 无可执行操作
}

bool KBattleAI::SoldierToAttack()
{
	KCardInstList* lst = QueryCardSet(KCardInst::enum_slot_fight);

	KCardInst* pSelectCard = NULL;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardInst* pCard = *it;
		if(!pCard->m_attr.getReady()) continue;
		if(pCard->FindBuf(KAbilityStatic::what_dist)){
			pSelectCard = pCard;
			break;
		}
		if(pSelectCard && pSelectCard->GetAtk() >=pCard->GetAtk()) continue;
		pSelectCard = pCard;
	}

	if(!pSelectCard) return false;

	KBattleGuy* pDefGuy = m_battleCtrl->GetDefGuy();
	KCardInstList enemyGuider;
	KCardInst* target = NULL;
	pDefGuy->GetDeck().FindFightingGuider(&enemyGuider);
	if(enemyGuider.size()>0){
		target = enemyGuider.front();
	}else{
		KCardInstList lst;
		pDefGuy->GetDeck().GetDefenderSet(&lst);
		if(lst.empty()){
			target = pDefGuy->GetDeck().GetHero();
		}else{
			target = lst.front();
		}
	}
	m_battleCtrl->DoPlayerOpOK(pSelectCard->GetRealId(),target->GetRealId(),0);
	return true;
}

bool KBattleAI::UseHeroSkill()
{
	KCardInst* pHero = m_battleCtrl->GetCurGuy()->GetDeck().GetHero();
	if(pHero->GetCost()>m_attr.getCurRes()) return false;
	if(!pHero->m_attr.getReady()) return false;
	KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(pHero->GetCardId()*10);
	if(!pAbility) return false;
	KCardInstList lst;
	KSkillAssist::_fillAbilityTarget(m_battleCtrl,pHero,pAbility,&lst);
	if(lst.empty()) return false;
	KCardInst* pSelected = NULL;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		if(!pCard->HasBuf(pAbility)){
			pSelected = pCard;
			break;
		}
	}

	if(!pSelected) return false;
	m_battleCtrl->DoPlayerOpOK(pHero->GetRealId(),pSelected->GetRealId(),0);
	return true;
}

bool KBattleAI::UseSecretCard()
{
	int pos = m_Deck.GetEmptySecretSlot();
	if(pos<0) return false;

	KCardInstList* pHandArr = QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = pHandArr->begin();it!=pHandArr->end();++it){
		KCardInst* pCard = *it;
		if(pCard->GetCost()> m_attr.getCurRes()) continue;
		if(pCard->GetType()!=KCardStatic::card_secret) continue;
        m_battleCtrl->DoPlayerOpOK(pCard->GetRealId(),0,0);
        return true;
	}
    return false;
}

bool KBattleAI::UseSkillCard()
{
	KCardInstList* pHandArr = QueryCardSet(KCardInst::enum_slot_hand);

	for(KCardInstList::iterator it = pHandArr->begin();it!=pHandArr->end();++it){
		KCardInst* pCard = *it;
		int target = 0;
		if(pCard->GetCost()> m_attr.getCurRes()) continue;
		if(pCard->GetType()!=KCardStatic::card_skill) continue;
		if(IsUseSkillGood(pCard,target)){
			m_battleCtrl->DoPlayerOpOK(pCard->GetRealId(),target,0);
			return true;
		}
	}
	return false;
}

bool KBattleAI::IsUseSkillGood(KCardInst* pCard,int& target)
{
	KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(pCard->GetCardId()*10);
	if(!pAbility) return false;
	KCardInstList lst;
	KCardInstList lstMy;
	KSkillAssist::_fillAbilityTarget(m_battleCtrl,pCard,pAbility,&lst);
	KSkillAssist::_fillAbilityTarget(m_battleCtrl,pCard,pAbility,&lstMy,true);
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_damage:
		{
			int val1 = CalcTotalDamage(pAbility,lst,target);
			int val2 = CalcTotalDamage(pAbility,lstMy,target);
			return (val1 -2*val2) >= pCard->GetCost(); //对敌方伤害大
		}
		break;
	case KAbilityStatic::what_control:
		{
			return CalcControlBenefit(pAbility,lst,target) >=  pCard->GetCost();
		}
		break;
	case KAbilityStatic::what_heal:
		{
			int val1 = CalcTotalHeal(pAbility,lst,target);
			int val2 = CalcTotalHeal(pAbility,lstMy,target);
			return val2*2 >val1;  //对我方治疗量大
		}
		break;
	case KAbilityStatic::what_hp_double:
		{
			int val = CalcTotalHpDouble(pAbility,lstMy,target);
			return val > pCard->GetCost()*1.5;
		}
		break;
	case KAbilityStatic::what_copy_hand:
		{
			return true;
		}
		break;
	case KAbilityStatic::what_copy_fight:
		{
			int pos = m_Deck.GetEmptyFightSlot();
			return (pos >=0);
		}
		break;
    default:
        break;
	}
	return false;
}

int KBattleAI::CalcTotalHpDouble(KAbilityStatic* pAbility,KCardInstList& lst,int& target)
{
	int maxHp = 0;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		int val = pCard->GetHp();
		
		if(maxHp<val){
			maxHp = val;
			target = pCard->GetRealId();
		}
	}
	return maxHp;
}

int KBattleAI::CalcControlBenefit(KAbilityStatic* pAbility,KCardInstList& lst,int& target)
{
	int valMax = 0;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		int val = pCard->GetAtk() +pCard->GetHp();
		if(valMax<val){
				valMax = val;
				target = pCard->GetRealId();
		}
	}
	return valMax;
}

int KBattleAI::CalcTotalDamage(KAbilityStatic* pAbility,KCardInstList& lst,int& target)
{
	int totalDamage = 0;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		int val = (pCard->GetHp()>pAbility->GetVal())? pAbility->GetVal():pCard->GetHp();
		if(pAbility->IsArea()){
			totalDamage += val;
		}else{
			if(totalDamage<val){
				totalDamage = val;
				target = pCard->GetRealId();
			}
		}
	}
	return totalDamage;
}

int KBattleAI::CalcTotalHeal(KAbilityStatic* pAbility,KCardInstList& lst,int& target)
{
	int totalHeal = 0;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		int val = ( pCard->GetLostHp()> pAbility->GetVal())? pAbility->GetVal(): pCard->GetLostHp();
		if(pAbility->IsArea()){
			totalHeal += val;
		}else{
			if(totalHeal<val){
				totalHeal = val;
				target = pCard->GetRealId();
			}
		}
	}
	return totalHeal;
}

bool KBattleAI::HandCardToField()
{
    int pos = m_Deck.GetRndEmptyFightSlot();
    if(pos<0) return false;
	KCardInstList* pHandList = QueryCardSet(KCardInst::enum_slot_hand);
	
	KCardInst* pSelectCard = NULL;

	for(KCardInstList::iterator it = pHandList->begin();it!=pHandList->end();++it){
		KCardInst* pCard = *it;
		if(pCard->GetCost()> m_attr.getCurRes()) continue;
		if(pCard->GetType()!=KCardStatic::card_soldier) continue;
		if(pSelectCard && pSelectCard->GetCost()>=pCard->GetCost()) continue;

		pSelectCard = pCard;
	}

	if(pSelectCard){
		KAbilityStatic* pAbility = pSelectCard->FindBuf(KAbilityStatic::when_enter);
		int dest = 0;
		if(pAbility&& !pAbility->IsArea()){
			KCardInstList lst;
			KCardInstList lstMy;
			KSkillAssist::_fillAbilityTarget(m_battleCtrl,pSelectCard,pAbility,&lst);
			KSkillAssist::_fillAbilityTarget(m_battleCtrl,pSelectCard,pAbility,&lstMy,true);
			if(lst.size()>0){
				dest = (*lst.begin())->GetRealId();
			}else if(lstMy.size()>0){
				dest = (*lstMy.begin())->GetRealId();
			}
		}
		

		m_battleCtrl->DoPlayerOpOK(pSelectCard->GetRealId(),dest,pos);
		return true;
	}else{
		return false;
	}
}

