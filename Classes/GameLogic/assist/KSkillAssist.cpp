#include "KSkillAssist.h"
#include "../KBattleCtrlBase.h"
#include "../KBattleDeck.h"
#include "../KCardInst.h"
#include "../KBattleGuy.h"
#include "../../StaticTable/KAbilityStatic.h"
#include "../../common/KCommonObj.h"
#include "../KDynamicWorld.h"
#include "../../Inc/KLogicMsg.h"

namespace KSkillAssist
{

void _fillAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* card,KAbilityStatic* pAbility,KCardInstList* lstMy,KCardInstList* lstYour)
{
	KAbilityStatic::Enum_AblityType abilityType = pAbility->GetAbilityType();
	switch(pAbility->GetAbilityType()){
	case KAbilityStatic::ability_good:
		KSkillAssist::_fillAbilityTarget(ctrl,card,pAbility,lstMy,true);
		break;
	case KAbilityStatic::ability_bad:
		KSkillAssist::_fillAbilityTarget(ctrl,card,pAbility,lstYour);
		break;
	default:
		KSkillAssist::_fillAbilityTarget(ctrl,card,pAbility,lstMy,true);
		KSkillAssist::_fillAbilityTarget(ctrl,card,pAbility,lstYour);
		break;
	}
}

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
	case KAbilityStatic::which_null:
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
KCardInst* _findActiveSecret(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic::Enum_When when )
{
	KBattleGuy* pDefGuy = pDes->GetOwner();
	KCardInstList* secretList = pDefGuy->GetDeck().QueryCardSet(KCardInst::enum_slot_secret);
	KCardInst* pSelectSecret = NULL;
	for(KCardInstList::iterator it = secretList->begin();it!=secretList->end();++it){
		KCardInst* pSecret = *it;
		if(_checkSecretAbility(pSrc,pDes,pSecret,when)){
			if(!pSelectSecret || 
				pSelectSecret->GetHp()< pSecret->GetHp()){
				pSelectSecret = pSecret;
			}
		}
	}
	return pSelectSecret;
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
		case KAbilityStatic::which_mysoldier:
			{
				return (pDes->GetType()==KCardStatic::card_soldier);
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

bool _doSecretAbility(KBattleCtrlBase* ctrl,KCardInst* pSecret,KCardInst** pSrc,KCardInst** pDes)
{
	bool ret = false;
	KAbilityStatic* pSecretAbility = pSecret->GetSecretAbility();
	switch(pSecretAbility->GetWhat()){
	case KAbilityStatic::what_summon_guider:
		{
			int id = _summonCard(ctrl,pSecret,pSecretAbility,*pSrc);
			if(id>0){
				KCardInst* pNewDes = pSecret->GetOwner()->GetDeck().GetCard(id);
				*pDes = pNewDes;
			}
		}
		break;
	}
	pSecret->GetOwner()->GetDeck().onCard2Tomb(pSecret);
	return ret;
}


int _summonCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInst* pActor)
{
	int id = 0;
	KBattleGuy* pPlayer = pSrc->GetOwner();
	strCardAbilityResult result;
	if(pActor){
		result.init(pActor->GetRealId(),pSrc->GetRealId(),pAbility);
	}else{
		result.init(pSrc->GetRealId(),pSrc->GetRealId(),pAbility);
	}
	int emptySlotNum = pPlayer->GetDeck().GetEmptyFightSlotNum();
	if(emptySlotNum<0) return 0;
	int num = (pAbility->GetMax()>emptySlotNum)? emptySlotNum:pAbility->GetMax();
	for(int i=0;i<num;i++){
		id = pPlayer->GetDeck().SummonCard(pAbility->GetVal())->GetRealId();
		result.SetDestVal(id,0);
	}
	_sendAbilityResult(ctrl,result);
	return id;
}

void _sendAbilityResult(KBattleCtrlBase* ctrl,strCardAbilityResult& result)
{
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_ABILITYRESULT,(unsigned long long)&result,(unsigned long long)ctrl->GetWorld());
}

void _copyHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	KBattleGuy* pDef = ctrl->GetDefGuy();
	KCardInstList lst,newLst;
	pDef->GetDeck().RndPickCard(lst,pAbility->GetVal(),KCardInst::enum_slot_hand);
	pPlayer->GetDeck().CreateCloneCard(lst,newLst,KCardInst::enum_slot_hand);

	strCardAbilityResult result;
	result.init(pSrc->GetRealId(),pSrc->GetRealId(),pAbility);
	for(KCardInstList::iterator it = newLst.begin();it!=newLst.end();++it){
		result.SetDestVal((*it)->GetRealId(),0);
	}
	_sendAbilityResult(ctrl,result);
}

void _copyFightSoldier(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	KBattleGuy* pDef = ctrl->GetDefGuy();
	KCardInstList lst,newLst;
	pDef->GetDeck().RndPickCard(lst,1,KCardInst::enum_slot_slot,KCardStatic::card_soldier);
	pPlayer->GetDeck().CreateCloneCard(lst,newLst,KCardInst::enum_slot_fight);

	strCardAbilityResult result;
	result.init(pSrc->GetRealId(),pSrc->GetRealId(),pAbility);
	for(KCardInstList::iterator it = newLst.begin();it!=newLst.end();++it){
		result.SetDestVal((*it)->GetRealId(),0);
		KCardInst* card = ctrl->GetCard((*it)->GetRealId());
		pPlayer->GetDeck().Hand2Fight(card);
	}
	_sendAbilityResult(ctrl,result);
}

}