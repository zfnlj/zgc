#include "KSkillAssist.h"
#include "../KBattleCtrlBase.h"
#include "../KBattleDeck.h"
#include "../KCardInst.h"
#include "../KBattleGuy.h"
#include "../../StaticTable/KAbilityStatic.h"
#include "../../common/KCommonObj.h"

namespace KSkillAssist
{

void _fillAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lst,bool bMy)
{
	KBattleDeck& MyDeck = ctrl->GetCurGuy()->GetDeck();
	KBattleDeck& YourDeck = ctrl->GetDefGuy()->GetDeck();
	KCardInst* skip = (pAbility->ToSelfEnable())?NULL:pSrc;
	KCardInstList tmpLst;
	switch(pAbility->GetWhich()){
	case KAbilityStatic::which_mysoldier:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_myhero:
		{
			tmpLst.push_back(MyDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_my:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(MyDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_yoursoldier:
		{
			if(!bMy) YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_yourhero:
		{
			if(!bMy) tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_your:
		{
			if(!bMy){
				YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
				tmpLst.push_back(YourDeck.GetHero());
			}
		}
		break;
	case KAbilityStatic::which_soldier:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			if(!bMy) YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_hero:
		{
			tmpLst.push_back(MyDeck.GetHero());
			if(!bMy) tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_all:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(MyDeck.GetHero());
			if(!bMy) YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			if(!bMy) tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_i:
		tmpLst.push_back(pSrc);
		break;
	case KAbilityStatic::which_u:
		break;
	case KAbilityStatic::which_owner:
		break;
	default:
		ASSERT(false);
		//CCAssert(false , "KBattleGod::GetAbilityTarget error");
		break;
	}
	for(KCardInstList::iterator it = tmpLst.begin(); it!=tmpLst.end();++it){
		KCardInst* card = *it;
		if(card->HasBuf(pAbility)) continue; //same buf only one..
		if(pAbility->GetWhat()==KAbilityStatic::what_kill_atk_le){
			if(pAbility->GetVal()<card->GetAtk()) continue;
		}else if(pAbility->GetWhat()==KAbilityStatic::what_kill_atk_he){
			if(card->GetAtk()<pAbility->GetVal()) continue;
		}
		lst->push_back(card);
	}
}

void _rndFillProc(KAbilityStatic* pAbility,KCardInstList* lst)
{
	if(!pAbility->IsRnd()) return;
	if(pAbility->GetWhat()!= KAbilityStatic::what_kill &&
		pAbility->GetWhat()!= KAbilityStatic::what_heal) return;


	int pickNum = (pAbility->GetRnd()>lst->size())? lst->size():pAbility->GetRnd();
	if(pickNum==lst->size()) pickNum--;

	while(lst->size()>pickNum){
		int nRand = g_rnd.GetRandom(0,lst->size());
		int pos=0;
		for(KCardInstList::iterator it=lst->begin();it!=lst->end();it++,pos++){
			if(nRand==pos){
				lst->erase(it);
				break;
			}
		}
	}

	
}

bool _checkSecretAbility(KCardInst* pSrc,KCardInst* pDes,KCardInst* pSecret,KAbilityStatic::Enum_When when )
{
	if(!pSecret) return false;
	KAbilityStatic* pSecretAbility = pSecret->GetSecretAbility();
	if(!pSecretAbility || pSecretAbility->GetWhen()!= when) return false;

	switch(pSecretAbility->GetWhich()){
		case KAbilityStatic::which_myhero:
			{
				return (pDes->GetType()==KCardStatic::card_hero);
			}
			break;
		default:
			break;
	}
	return false;
}

void _removeBuf(KCardAbilityList& list,int id)
{
	KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(id);
	if(!pBuf) return;
	KCardAbilityList::iterator it = list.begin();
	while(it != list.end()){
		if(*it == pBuf){
			list.erase(it);
			return;
		}
		it++;
	}
}

KAbilityStatic* _findBuf(KCardAbilityList& list,KAbilityStatic::Enum_What what)
{
	KCardAbilityList::iterator it = list.begin();
	while(it != list.end()){
		if((*it)->GetWhat()== what) return *it;
		it++;
	}
	return NULL;
}

}