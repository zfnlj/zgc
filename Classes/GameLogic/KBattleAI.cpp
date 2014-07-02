#include "KBattleAI.h"
#include "KBattleCtrlBase.h"
#include "KBattleGod.h"
#include "KDynamicWorld.h"
#include "../StaticTable/KAbilityStatic.h"
#include "assist/KSkillAssist.h"
#include "assist/KAIAssist.h"

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
	KCardInstList* handList = m_Deck.QueryCardSet(KCardInst::enum_slot_hand);
	KCardInstList tmpLst;
	for(KCardInstList::iterator it = handList->begin(); it!=handList->end();++it){
		if((*it)->GetCost()>3) tmpLst.push_back(*it);
	}
	KCardInstList* slotList = m_Deck.QueryCardSet(KCardInst::enum_slot_slot); // move cost>3 card to the end of slot list;
	int n=0;
	for(KCardInstList::iterator it = slotList->begin(); it!=slotList->end();n++){
		KCardInst* card = *it;
		if(n>=3) break;
		if((*it)->GetCost()>3){
			it = slotList->erase(it);
			slotList->push_back(card);
		}else{
			++it;
		}
	}

	DoSelectBeginCard(&tmpLst);
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

bool KBattleAI::ThinkePlayCard()
{
	KCardInstList* myFightLst = QueryCardSet(KCardInst::enum_slot_fight);
	int myVal = KAIAssist::_calcTotalCardVal(*myFightLst);

	KBattleGuy* pDefGuy = m_battleCtrl->GetDefGuy();
	KCardInstList* yourFightLst = pDefGuy->QueryCardSet(KCardInst::enum_slot_fight);
	int yourVal = KAIAssist::_calcTotalCardVal(*yourFightLst);
	if(myVal<=yourVal){
		if(HandCardToField()) return true;
		if(UseSkillCard()) return true;
	}else{
		if(UseSkillCard()) return true;
		if(HandCardToField()) return true;
	}
	
	return false;
}

void KBattleAI::ThinkToPlay(float dt)
{
	m_thinkElapsed += dt;
	if(m_thinkElapsed<5)
		return;
	if(m_battleCtrl->IsWaitDrama())
		return;
	m_thinkElapsed = 0;
	
	if(ThinkePlayCard()) return;
	//if(UseHeroSkill())
	//	return;
	
    /*if(UseSecretCard())
        return;*/
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
		if(pCard->GetAtk()==0) continue;
		if(pCard->FindRealBuf(KAbilityStatic::what_dist)){
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
		pDefGuy->GetDeck().GetDefenderSet(&enemyGuider);
		enemyGuider.push_back(pDefGuy->GetDeck().GetHero());

		target = KAIAssist::_BestAttackTarget(pSelectCard,enemyGuider);
		if(!target) return false;
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
	KSkillAssist::_fillAllAbilityTarget(m_battleCtrl,pHero,NULL,pAbility,&lst);
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
		if(!pCard->IsKindOf(KCardStatic::card_secret)) continue;
        m_battleCtrl->DoPlayerOpOK(pCard->GetRealId(),0,0);
        return true;
	}
    return false;
}

bool KBattleAI::UseSkillCard()
{
	KCardInstList* pHandArr = QueryCardSet(KCardInst::enum_slot_hand);

	KCardInstList skillLst;
	for(KCardInstList::iterator it = pHandArr->begin();it!=pHandArr->end();++it){
		KCardInst* pCard = *it;
		int target = 0;
		if(pCard->GetCost()> m_attr.getCurRes()) continue;
		if(!pCard->IsKindOf(KCardStatic::card_skill)) continue;
		skillLst.push_back(pCard);
	}
	KAIAssist::_sortOnAbilityPriority(skillLst);

	for(KCardInstList::iterator it = skillLst.begin();it!=skillLst.end();++it){
		KCardInst* pCard = *it;
		int target = 0;
		KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(pCard->GetCardId()*10); 
		if(pAbility->IsArea()){
			if(IsUseRangeSkillGood(pCard,pAbility)){
				m_battleCtrl->DoPlayerOpOK(pCard->GetRealId(),0,0);
				return true;
			}
		}else{
			if(IsUseTargetSkillGood(pCard,pAbility,target)){
				m_battleCtrl->DoPlayerOpOK(pCard->GetRealId(),target,0);
				return true;
			}
		}
	}
	return false;
}

bool KBattleAI::IsUseSoldierAbilityGood(KCardInst* pCard,int& target)
{
	KAbilityStatic* pAbility = KSkillAssist::_findStaticAbility(pCard->GetCardId(),KAbilityStatic::when_enter);

	if(!pAbility) return false;
	if(pAbility->GetWhich()==KAbilityStatic::which_owner) return true;

	KCardInstList lst;
	KCardInstList lstMy;
	KSkillAssist::_fillYourAbilityTarget(m_battleCtrl,pCard,NULL,pAbility,&lst);
	KSkillAssist::_fillMyAbilityTarget(m_battleCtrl,pCard,NULL,pAbility,&lstMy);
	KCardInst* pBest = NULL;
	int maxVal = 0;
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_damage:
	case KAbilityStatic::what_control:
	case KAbilityStatic::what_stun:
	case KAbilityStatic::what_kill:
	case KAbilityStatic::what_hp_set:
		pBest = KAIAssist::_MostValuableTarget(lst);
		break;
	case KAbilityStatic::what_guide:
	case KAbilityStatic::what_hide:
	case KAbilityStatic::what_dist:
		{
			pBest = KAIAssist::_MostValuableTargetNoBuf(lstMy, pAbility->GetWhat());
		}
		break;
	case KAbilityStatic::what_heal:
	case KAbilityStatic::what_atk_set:
	case KAbilityStatic::what_atk_equ_hp:
		{
			pBest = KAIAssist::_MostAbilityDoValTarget(m_battleCtrl,pAbility,pCard,&lstMy,maxVal);
		}
		break;
	case KAbilityStatic::what_atkhp_sw:
	case KAbilityStatic::what_damage_atkadd:
		{
			int val = 0;
			pBest = KAIAssist::_MostAbilityDoValTarget(m_battleCtrl,pAbility,pCard,&lstMy,&lst,val);
		}
		break;
	case KAbilityStatic::what_atk_add:
	case KAbilityStatic::what_add_atk_hp:
	case KAbilityStatic::what_hp_add:
	case KAbilityStatic::what_hp_double:
	case KAbilityStatic::what_immune:
		{
			pBest = KAIAssist::_MostValuableTarget(lstMy);
			break;
		}
		break;
	case KAbilityStatic::what_rush:
		{
			pBest = KAIAssist::_MostValuableTargetExistBuf(lstMy,KAbilityStatic::what_can_rush);
		}
		break;
	case KAbilityStatic::what_dispel_buf:
		{
			pBest = KAIAssist::_MostValuableBufTarget(lst);
		}
		break;
	default:
		CCAssert(false , "TBD!");
        break;
	}
	if(pBest){
		target = pBest->GetRealId();
		return  true;
	}else{
		return false;
	}

}

bool KBattleAI::IsUseRangeSkillGood(KCardInst* pCard,KAbilityStatic* pAbility)
{
	if(!pAbility) return false;
	KCardInstList lst;
	KCardInstList lstMy;
	KSkillAssist::_fillYourAbilityTarget(m_battleCtrl,pCard,NULL,pAbility,&lst);
	KSkillAssist::_fillMyAbilityTarget(m_battleCtrl,pCard,NULL,pAbility,&lstMy);
	int target=0;
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_damage:
	case KAbilityStatic::what_kill:
	case KAbilityStatic::what_heal:
	case KAbilityStatic::what_atk_add:
	case KAbilityStatic::what_hp_set:
	case KAbilityStatic::what_atk_set:
	case KAbilityStatic::what_atk_equ_hp:
		{
			int v1 = KAIAssist::_calcTotalAbilityDoVal(m_battleCtrl,pAbility,pCard,lst); 
			int v2 = KAIAssist::_calcTotalAbilityDoVal(m_battleCtrl,pAbility,pCard,lstMy);
			return (v2 -v1) > pCard->GetCost()*10; //对敌方伤害大
		}
		break;
	case KAbilityStatic::what_stun:
		{
			return lst.size()>lstMy.size();
		}
		break;
	case KAbilityStatic::what_draw_card:
		{
			 return KSkillAssist::_calcValDef(m_battleCtrl,this,pCard,pAbility->GetVal())>0;
		}
		break;
	case KAbilityStatic::what_copy_hand:
	case KAbilityStatic::what_res_add:
		{
			return true;
		}
		break;
	case KAbilityStatic::what_copy_fight:
	case KAbilityStatic::what_summon:
	case KAbilityStatic::what_summon_guider:
		{
			int pos = m_Deck.GetEmptyFightSlot();
			return (pos >=0);
		}
		break;
    default:
		CCAssert(false , "TBD!");
        break;
	}
	return false;
}

bool KBattleAI::IsUseTargetSkillGood(KCardInst* pCard,KAbilityStatic* pAbility,int& target)
{
	if(!pAbility) return false;
	KCardInstList lst;
	KCardInstList lstMy;
	KSkillAssist::_fillYourAbilityTarget(m_battleCtrl,pCard,NULL,pAbility,&lst);
	KSkillAssist::_fillMyAbilityTarget(m_battleCtrl,pCard,NULL,pAbility,&lstMy);
	KCardInst* pBest = NULL;
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_stun:
		{
			pBest = KAIAssist::_MostValuableTarget(lst);
			if(pBest && KAIAssist::_calcCardValue(pBest)<8*pCard->GetCost()) pBest=NULL;
		}
		break;
	case KAbilityStatic::what_return:
		{
			pBest = KAIAssist::_MostValuableTarget(lst);
			if(pBest &&KAIAssist::_calcCardValue(pBest)<10*pCard->GetCost()) pBest=NULL;
		}
		break;
	case KAbilityStatic::what_replace:
		{
			pBest = KAIAssist::_MostValuableTarget(lst);
			if(pBest && KAIAssist::_calcCardValue(pBest)<9*pCard->GetCost()) pBest=NULL;
		}
		break;
	case KAbilityStatic::what_kill:
		{
			pBest = KAIAssist::_MostValuableTarget(lst);
			if(pBest && KAIAssist::_calcCardValue(pBest)<7*pCard->GetCost()) pBest=NULL;
		}
		break;
	case KAbilityStatic::what_control:
		{
			pBest = KAIAssist::_MostValuableTarget(lst);
			if(pBest && KAIAssist::_calcCardValue(pBest)<4*pCard->GetCost()) pBest=NULL;
		}
		break;
	case KAbilityStatic::what_hp_set:
	case KAbilityStatic::what_atk_set:
	case KAbilityStatic::what_atk_equ_hp:
	case KAbilityStatic::what_damage_atkadd:
	case KAbilityStatic::what_atkhp_sw:
	case KAbilityStatic::what_damage:
	case KAbilityStatic::what_heal:
		{
			int val = 0;
			pBest = KAIAssist::_MostAbilityDoValTarget(m_battleCtrl,pAbility,pCard,&lstMy,&lst,val);
			if(pBest && val < 8*pCard->GetCost()) pBest = NULL;
		}
		break;
	case KAbilityStatic::what_rush:
		{
			pBest = KAIAssist::_MostValuableTargetExistBuf(lstMy,KAbilityStatic::what_can_rush);
		}
		break;
	case KAbilityStatic::what_hp_double:
	case KAbilityStatic::what_hp_add:
	case KAbilityStatic::what_atk_add:
	case KAbilityStatic::what_add_atk_hp:
	case KAbilityStatic::what_immune:
		{
			pBest = KAIAssist::_MostValuableTarget(lstMy);
		}
		break;
	case KAbilityStatic::what_hide:
	case KAbilityStatic::what_dist:
	case KAbilityStatic::what_guide:
		{
			pBest = KAIAssist::_MostValuableTargetNoBuf(lstMy, pAbility->GetWhat());
		}
		break;
	case KAbilityStatic::what_dispel_buf:
		{
			pBest = KAIAssist::_MostValuableBufTarget(lst);
		}
		break;
    default:
		CCAssert(false , "TBD!");
        break;
	}

	if(!pBest) return false;
	target = pBest->GetRealId();
	return true;
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
		if(!pCard->IsKindOf(KCardStatic::card_soldier)) continue;
		if(pSelectCard && pSelectCard->GetCost()>=pCard->GetCost()) continue;

		pSelectCard = pCard;
	}

	if(pSelectCard){
		int dest = 0;
		if(KSkillAssist::_needEnterFightTarget(pSelectCard->GetCardId())){
			if(!IsUseSoldierAbilityGood(pSelectCard,dest)) dest=0;
		}

		m_battleCtrl->DoPlayerOpOK(pSelectCard->GetRealId(),dest,pos);
		return true;
	}else{
		return false;
	}
}

