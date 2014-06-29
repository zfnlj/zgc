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


void KBattleAI::ThinkToPlay(float dt)
{
	m_thinkElapsed += dt;
	if(m_thinkElapsed<5)
		return;
	if(m_battleCtrl->IsWaitDrama())
		return;
	m_thinkElapsed = 0;
	if(HandCardToField())
		return;
	if(UseSkillCard())
		return;
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

	for(KCardInstList::iterator it = pHandArr->begin();it!=pHandArr->end();++it){
		KCardInst* pCard = *it;
		int target = 0;
		if(pCard->GetCost()> m_attr.getCurRes()) continue;
		if(!pCard->IsKindOf(KCardStatic::card_skill)) continue;
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
	case KAbilityStatic::what_return:
	case KAbilityStatic::what_kill:
	case KAbilityStatic::what_hp_set:
		pBest = KAIAssist::_MostValuableTarget(lst);
		break;
	case KAbilityStatic::what_dispel_buf:
		{
			CCAssert(false , "TBD!");
		}
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
			pBest = KAIAssist::_MostAbilityDoValTarget(m_battleCtrl,pAbility,pCard,lstMy,maxVal);
		}
		break;
	case KAbilityStatic::what_atkhp_sw:
		{
			pBest = KAIAssist::_MostAbilityDoValTarget(m_battleCtrl,pAbility,pCard,lstMy,lst);
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
	case KAbilityStatic::what_damage_atkadd:
		{
			return PickDamageAtkAdd(pCard,pAbility,lstMy,lst,target);
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

bool KBattleAI::PickDamageAtkAdd(KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList& lstMy,KCardInstList& lst,int& target)
{
	KCardInst* pMySoldier =KAIAssist::_MostValuableTarget(lstMy,99,pAbility->GetNormalVal()+2);
	if(pMySoldier){
		target = pMySoldier->GetRealId();
		return true;
	}
	KCardInst* pYourSoldier =KAIAssist::_MostValuableTarget(lstMy,pAbility->GetNormalVal());
	if(pYourSoldier){
		target = pYourSoldier->GetRealId();
		return  KAIAssist::_calcCardValue(pYourSoldier)>pSrc->GetCost()*8;
	}
	return false;
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
		{
			int val1 = CalcTotalDamage(pAbility,lst,target);
			int val2 = CalcTotalDamage(pAbility,lstMy,target);
			return (val1 -2*val2) >= pCard->GetCost(); //对敌方伤害大
		}
		break;
	case KAbilityStatic::what_heal:
		{
			int val1 = CalcTotalHeal(pAbility,lst,target);
			int val2 = CalcTotalHeal(pAbility,lstMy,target);
			return val2*2 >val1;  //对我方治疗量大
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
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_damage:
	case KAbilityStatic::what_kill:
	case KAbilityStatic::what_stun:
	case KAbilityStatic::what_control:
	case KAbilityStatic::what_hp_set:
	case KAbilityStatic::what_replace:
	case KAbilityStatic::what_return:
		{
			KCardInst* pBest = KAIAssist::_MostValuableTarget(lst);
			if(!pBest) return false;
			target = pBest->GetRealId();
			return  KAIAssist::_calcCardValue(pBest)>pCard->GetCost()*8;
		}
		break;
	case KAbilityStatic::what_heal:
		{
			int val = CalcTotalHeal(pAbility,lstMy,target);
			return val*10 > pCard->GetCost()*15;  //对我方治疗量大
		}
		break;
	case KAbilityStatic::what_hp_double:
	case KAbilityStatic::what_hp_add:
	case KAbilityStatic::what_atk_add:
	case KAbilityStatic::what_add_atk_hp:
	case KAbilityStatic::what_immune:
	case KAbilityStatic::what_rush:
		{
			KCardInst* pBest = KAIAssist::_MostValuableTarget(lstMy);
			if(!pBest) return false;
			target = pBest->GetRealId();
			return  KAIAssist::_calcCardValue(pBest)>pCard->GetCost()*8;
		}
		break;
	case KAbilityStatic::what_atk_set:
		{
			return PickAtkSetTarget(pCard,pAbility,lstMy,lst,target);
		}
		break;
	case KAbilityStatic::what_atk_equ_hp:
		{
			return PicAtkEquHpTarget(pCard,pAbility,lstMy,lst,target);
		}
		break;
	case KAbilityStatic::what_damage_atkadd:
		{
			return PickDamageAtkAdd(pCard,pAbility,lstMy,lst,target);
		}
		break;
    default:
		CCAssert(false , "TBD!");
        break;
	}
	return false;
}

bool KBattleAI::PicAtkEquHpTarget(KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList& lstMy,KCardInstList& lst,int& target)
{
	KCardInst* pMyBest = KAIAssist::_MostHpTarget(lstMy);
	int v1 = KAIAssist::_calcAbilityDoVal(m_battleCtrl,pAbility,pSrc,pMyBest);

	KCardInst* pYourBest =KAIAssist::_MostValuableTarget(lst,1);
	int v2 = -KAIAssist::_calcAbilityDoVal(m_battleCtrl,pAbility,pSrc,pYourBest);
	if( v1 > v2 ){
		if( v1 > pSrc->GetCost()*8){
			target = pMyBest->GetRealId();
			return true;
		}
	}else{
		if( v2 > pSrc->GetCost()*8){
			target = pYourBest->GetRealId();
			return true;
		}
	}
	return false;
}

bool KBattleAI::PickAtkSetTarget(KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList& lstMy,KCardInstList& lst,int& target)
{
	KCardInst* pMyBest = KAIAssist::_MostHpTarget(lstMy);
	int v1 = KAIAssist::_calcAbilityDoVal(m_battleCtrl,pAbility,pSrc,pMyBest);

	KCardInst* pYourBest = KAIAssist::_MostHpTarget(lst);
	int v2 = -KAIAssist::_calcAbilityDoVal(m_battleCtrl,pAbility,pSrc,pYourBest);
	if( v1 > v2 ){
		if( v1 > pSrc->GetCost()*8){
			target = pMyBest->GetRealId();
			return true;
		}
	}else{
		if( v2 > pSrc->GetCost()*8){
			target = pYourBest->GetRealId();
			return true;
		}
	}
	return false;
}

void KBattleAI::PickMaxValTarget(KCardInstList& lst,int& target)
{
	int maxVal = 0;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		int val = pCard->GetHp() + pCard->GetAtk();
		if(val>maxVal){
			target = pCard->GetRealId();
			maxVal = val;
		}
	}
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

int KBattleAI::CalcControlBenefit(KCardInstList& lst,int& target)
{
	int valMax = 0;
	KCardInst* pBest = KAIAssist::_MostValuableTarget(lst);
	if(!pBest) return 0;
	target = pBest->GetRealId();
	return pBest->GetAtk() +pBest->GetHp();
}

int KBattleAI::CalcTotalDamage(KAbilityStatic* pAbility,KCardInstList& lst,int& target)
{
	int totalDamage = 0;
	for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
		KCardInst* pCard = *it;
		int val=0;
		switch(pAbility->GetWhat()){
			case KAbilityStatic::what_damage:
				val = (pCard->GetHp()>pAbility->GetNormalVal())? pAbility->GetNormalVal():pCard->GetHp();
				break;
			case KAbilityStatic::what_kill:
				val = pCard->GetHp();
				break;
			default:
				break;
		}
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
		int val = ( pCard->GetLostHp()> pAbility->GetNormalVal())? pAbility->GetNormalVal(): pCard->GetLostHp();
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

