#include "KBattleCtrlAssist.h"
#include "../KBattleGuy.h"
#include "../KBattleCtrlBase.h"
#include "KSkillAssist.h"

KCardInst* KBattleCtrlAssist::GetCard(KBattleCtrlBase* ctrl,int id)
{
	if(id<=0) return NULL;
	for(KBattleGuyList::iterator it = ctrl->m_BattleGuyList.begin();it!=ctrl->m_BattleGuyList.end();it++){
		KCardInst* pCard = (*it)->GetDeck().GetCard(id);
		if(pCard){
			return pCard;
		}
	}
	return NULL;
}

KBattleGuy* KBattleCtrlAssist::GetOtherGuy(KBattleCtrlBase* ctrl,UINT64 id)
{
	for(KBattleGuyList::iterator it = ctrl->m_BattleGuyList.begin();it!=ctrl->m_BattleGuyList.end();it++){
		if((*it)->GetGuyId()!=id) return *it;
	}
	return NULL;
}

KBattleGuy* KBattleCtrlAssist::GetGuy(KBattleCtrlBase* ctrl,UINT64 id)
{
	for(KBattleGuyList::iterator it = ctrl->m_BattleGuyList.begin();it!=ctrl->m_BattleGuyList.end();it++){
		if((*it)->GetGuyId()==id) return *it;
	}
	return NULL;
}

bool KBattleCtrlAssist::QueryEnterFightTarget(KBattleCtrlBase* ctrl,KCardInst*  card,KCardInstList* arrGreen,KCardInstList* arrRed)
{
	if(!card->IsKindOf(KCardStatic::card_soldier)) return false;
	if(card->GetSlot()!=KCardInst::enum_slot_hand) return false;
	if(ctrl->m_CurOp._slot<0) return false;
	KAbilityStatic* pAbility = KSkillAssist::_findStaticAbility(card->GetCardId(),KAbilityStatic::when_enter);
	if(!pAbility) return false;
	KSkillAssist::_fillAllAbilityTarget(ctrl,card,pAbility,arrGreen,arrRed);
	return true;
}

void KBattleCtrlAssist::QuerySkillTarget(KBattleCtrlBase* ctrl,KCardInst* skill,KCardInstList* arrGreen,KCardInstList* arrRed)
{
	if(!skill->IsKindOf(KCardStatic::card_skill) &&
		!skill->IsKindOf(KCardStatic::card_hero))
		return;
	KCardAbilityList abilityList;
	KGameStaticMgr::getSingleton().GetAbilityList(skill->GetCardId(),abilityList);
	for(KCardAbilityList::iterator it=abilityList.begin();it!=abilityList.end();++it){
		KAbilityStatic* pAbility = *it;
		KSkillAssist::_fillAllAbilityTarget(ctrl,skill,pAbility,arrGreen,arrRed);
	}
}

KCardInstList* KBattleCtrlAssist::GetCardSet(KBattleCtrlBase* ctrl,KCardInst* card)
{
	KCardInstList* lst = ctrl->m_pMainPlayer->GetDeck().QueryCardSet(card->GetSlot());
	if(_findCardIt(lst,card)!=lst->end()) return lst;

	lst = ctrl->GetOtherPlayer()->QueryCardSet(card->GetSlot());
	if(_findCardIt(lst,card)!=lst->end()) return lst;
	CCAssert(false , "Error to  GetCardSet!");
	return NULL;
}

