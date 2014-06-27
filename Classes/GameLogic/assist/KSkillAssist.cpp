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

void _fillCtrlCardEvt(KBattleCtrlBase* ctrl,KCardInst* pCard,KAbilityStatic::Enum_When when)
{
	KBattleDeck& MyDeck = pCard->GetOwner()->GetDeck();
	
	KCardInstList tmpLst;
	MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,NULL);
	tmpLst.push_back(MyDeck.GetHero());
	for(KCardInstList::iterator it=tmpLst.begin();it!=tmpLst.end();++it)
	{
		KAbilityStatic::Enum_When realWhen = when;
		KCardInst* card = *it;
		KAbilityStatic* pAbility = card->FindBufAbility(when);
		if(!pAbility){
			if(when==KAbilityStatic::when_soldier_dead||
				when==KAbilityStatic::when_soldier_hurted||
				when==KAbilityStatic::when_hero_hurted||
				when==KAbilityStatic::when_soldier_healed||
				when==KAbilityStatic::when_use_skill||
				when==KAbilityStatic::when_use_card){
				realWhen = KAbilityStatic::Enum_When ((int)when+ 1);
			}
			pAbility = card->FindBufAbility(realWhen);
		}
		if(!pAbility) continue;
		if(!pAbility->ToSelfEnable() && (pCard==card)) continue;
		ctrl->AddCardEvtAbility(card,realWhen);
	}
	tmpLst.clear();
	KBattleDeck& YourDeck = ctrl->GetOtherGuy(pCard->GetOwner()->GetGuyId())->GetDeck();
	YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,NULL);
	tmpLst.push_back(YourDeck.GetHero());
	
	for(KCardInstList::iterator it=tmpLst.begin();it!=tmpLst.end();++it)
	{
		KCardInst* card = *it;
		KAbilityStatic* pAbility = card->FindBufAbility(when);
		if(!pAbility) continue;
		if(!pAbility->ToSelfEnable() && (pCard==card)) continue;
		ctrl->AddCardEvtAbility(card,when);
	}

}

void _filterAbility(KCardAbilityList* srcLst,KCardAbilityList* desLst,KAbilityStatic::Enum_When when)
{
	for(KCardAbilityList::iterator it = srcLst->begin();it != srcLst->end();++it){
		KAbilityStatic* pAbility = *it;
		if(pAbility->GetWhen()==when) desLst->push_back(pAbility);
	}
}

void _fillAllAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* card,KAbilityStatic* pAbility,KCardInstList* lstMy,KCardInstList* lstYour)
{
	KAbilityStatic::Enum_AblityType abilityType = pAbility->GetAbilityType();
	switch(pAbility->GetAbilityType()){
	case KAbilityStatic::ability_good:
		KSkillAssist::_fillMyAbilityTarget(ctrl,card,NULL,pAbility,lstMy);
		break;
	case KAbilityStatic::ability_bad:
		KSkillAssist::_fillYourAbilityTarget(ctrl,card,NULL,pAbility,lstYour);
		break;
	default:
		KSkillAssist::_fillMyAbilityTarget(ctrl,card,NULL,pAbility,lstMy);
		KSkillAssist::_fillYourAbilityTarget(ctrl,card,NULL,pAbility,lstYour);
		break;
	}
}

void _fillAllAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic* pAbility,KCardInstList* lst)
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
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_yourhero:
		{
			tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_your:
		{
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_src_nearby:
		{
			pSrc->GetOwner()->GetDeck().PickFighterNearby(&tmpLst,pSrc);
		}
		break;
	case KAbilityStatic::which_des_nearby:
		{
			if(pDes) pDes->GetOwner()->GetDeck().PickFighterNearby(&tmpLst,pDes);
		}
		break;
	case KAbilityStatic::which_soldier:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_hero:
		{
			tmpLst.push_back(MyDeck.GetHero());
			tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_all:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(MyDeck.GetHero());
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(YourDeck.GetHero());
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
		//CCAssert(false , "KBattleGod::GetAbilityTarget error");
		break;
	}
	for(KCardInstList::iterator it = tmpLst.begin(); it!=tmpLst.end();++it){
		KCardInst* card = *it;
		if(pAbility->GetWhat()== KAbilityStatic::what_buf){
			KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(pAbility->GetNormalVal());
			if(card->HasBuf(pBuf)) continue; //same buf only one..
		}
		if(card->HasBuf(pAbility)) continue; //same buf only one..
		if(!_IsMatch(pAbility->GetCond(),card)) continue;
		lst->push_back(card);
	}
}

void _fillMyAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic* pAbility,KCardInstList* lst)
{
	KBattleDeck& MyDeck = ctrl->GetCurGuy()->GetDeck();
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
	case KAbilityStatic::which_soldier:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_src_nearby:
		{
			pSrc->GetOwner()->GetDeck().PickFighterNearby(&tmpLst,pSrc);
		}
		break;
	case KAbilityStatic::which_des_nearby:
		{
			if(pDes) pDes->GetOwner()->GetDeck().PickFighterNearby(&tmpLst,pDes);
		}
		break;
	case KAbilityStatic::which_hero:
		{
			tmpLst.push_back(MyDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_all:
		{
			MyDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(MyDeck.GetHero());
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
		break;
	}
	for(KCardInstList::iterator it = tmpLst.begin(); it!=tmpLst.end();++it){
		KCardInst* card = *it;
		if(pAbility->GetWhat()== KAbilityStatic::what_buf){
			KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(pAbility->GetNormalVal());
			if(card->HasBuf(pBuf)) continue; //same buf only one..
		}
		if(pAbility->GetWhat()== KAbilityStatic::what_rush){
			if(!card->FindRealBuf(KAbilityStatic::what_can_rush)) continue;
		}
		if(card->HasBuf(pAbility)) continue; //same buf only one..
		if(!_IsMatch(pAbility->GetCond(),card)) continue;
		lst->push_back(card);
	}
}

void _fillYourAbilityTarget(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic* pAbility,KCardInstList* lst)
{
	KBattleDeck& YourDeck = ctrl->GetDefGuy()->GetDeck();
	KCardInst* skip = (pAbility->ToSelfEnable())?NULL:pSrc;
	KCardInstList tmpLst;
	switch(pAbility->GetWhich()){
	case KAbilityStatic::which_yoursoldier:
		{
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_yourhero:
		{
			tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_your:
		{
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_soldier:
		{
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
		}
		break;
	case KAbilityStatic::which_src_nearby:
		{
			pSrc->GetOwner()->GetDeck().PickFighterNearby(&tmpLst,pSrc);
		}
		break;
	case KAbilityStatic::which_des_nearby:
		{
			if(pDes) pDes->GetOwner()->GetDeck().PickFighterNearby(&tmpLst,pDes);
		}
		break;
	case KAbilityStatic::which_hero:
		{
			tmpLst.push_back(YourDeck.GetHero());
		}
		break;
	case KAbilityStatic::which_all:
		{
			YourDeck.PickCard(&tmpLst,KCardInst::enum_slot_fight,skip);
			tmpLst.push_back(YourDeck.GetHero());
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
		break;
	}
	for(KCardInstList::iterator it = tmpLst.begin(); it!=tmpLst.end();++it){
		KCardInst* card = *it;
		if(pAbility->GetWhat()== KAbilityStatic::what_buf){
			KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(pAbility->GetNormalVal());
			if(card->HasBuf(pBuf)) continue; //same buf only one..
		}
		if(card->HasBuf(pAbility)) continue; //same buf only one..
		if(!_IsMatch(pAbility->GetCond(),card)) continue;
		lst->push_back(card);
	}
}

void _rndFillProc(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,KCardInstList* lst)
{
	int maxNum = KSkillAssist::_calcValDef(ctrl,pSrc->GetOwner(),pSrc,pAbility->GetMax());

	int pickNum = (maxNum>(int)lst->size())? lst->size():maxNum;
	if(pickNum<0) pickNum=0;
	if(pickNum==lst->size()) return ;

	while((int)lst->size()>pickNum){
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
	KBattleGuy* pDefGuy = ctrl->GetDefGuy();
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
				return pDes->IsKindOf(KCardStatic::card_hero);
			}
			break;
		case KAbilityStatic::which_mysoldier:
			{
				return pDes->IsKindOf(KCardStatic::card_soldier);
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
			int id = _summonCard(ctrl,pSecret,pSecretAbility,(*pSrc)->GetRealId());
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

void _abilityGenCard(KBattleCtrlBase* ctrl,KBattleDeck* deck,KCardInst* pSrc,KAbilityStatic* pAbility,int actor)
{
	if(actor==0) actor = pSrc->GetRealId();
	KCardInst* card  = deck->CreateCard(pAbility->GetNormalVal(),KCardInst::enum_slot_hand);
	strCardAbilityResult result;
	result.init(actor,pSrc->GetRealId(),pAbility);
	result.SetDestVal(card->GetRealId(),0);
	_sendAbilityResult(ctrl,result);
}

int _summonCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor)
{
	if(actor==0) actor = pSrc->GetRealId();
	int id = 0;
	KBattleGuy* pPlayer = pSrc->GetOwner();
	strCardAbilityResult result;
	result.init(actor,pSrc->GetRealId(),pAbility);

	int emptySlotNum = pPlayer->GetDeck().GetEmptyFightSlotNum();
	if(emptySlotNum<0) return 0;
	int num = (pAbility->GetMaxNormalVal()>emptySlotNum)? emptySlotNum:pAbility->GetMaxNormalVal();
	for(int i=0;i<num;i++){
		id = pPlayer->GetDeck().SummonCard(pAbility->GetSummonId())->GetRealId();
		result.SetDestVal(id,0);
	}
	_sendAbilityResult(ctrl,result);
	return id;
}

void _sendAbilityResult(KBattleCtrlBase* ctrl,strCardAbilityResult& result)
{
	if(!result._pAbility->ActionIsEmpty()) ctrl->AddDramaElapsed(4.0f);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_ABILITYRESULT,(unsigned long long)&result,(unsigned long long)ctrl->GetWorld());
}

void _copyHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor)
{
	if(actor==0) actor = pSrc->GetRealId();
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	KBattleGuy* pDef = ctrl->GetDefGuy();
	KCardInstList lst,newLst;
	pDef->GetDeck().RndPickCard(lst,pAbility->GetNormalVal(),KCardInst::enum_slot_hand);
	pPlayer->GetDeck().CreateCloneCard(lst,newLst,KCardInst::enum_slot_hand);

	strCardAbilityResult result;
	result.init(actor,pSrc->GetRealId(),pAbility);
	for(KCardInstList::iterator it = newLst.begin();it!=newLst.end();++it){
		result.SetDestVal((*it)->GetRealId(),0);
	}
	_sendAbilityResult(ctrl,result);
}

void _copyFightSoldier(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor)
{
	if(actor==0) actor = pSrc->GetRealId();
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

bool _IsMatch(KConditionDef& condDef,KCardInst* card)
{
	switch(condDef.GetCond()){
	case KConditionDef::con_atk_he:
		return card->GetAtk() >= condDef.GetVal();
		break;
	case KConditionDef::con_atk_le:
		return card->GetAtk() <= condDef.GetVal();
		break;
	case KConditionDef::con_exist_buf:
		return (card->FindRealBuf((KAbilityStatic::Enum_What)condDef.GetVal())!=NULL);
		break;
	}
	return true;
}

int _calcValDef(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* card,KValDef& valDef)
{
	int ret =0;
	switch(valDef.GetDef()){
	case KValDef::val_my_hurted_soldierNum:
		ret = guy->GetDeck().GetHurtedSoldierNum();
		break;
	case KValDef::val_mySoldierNum:
		ret = guy->GetDeck().GetSoldierNum() + valDef._val;
		break;
	case KValDef::val_soldierNum:
		ret = ctrl->GetFighterNum() + valDef._val;
		break;
	case KValDef::val_rnd:
		ret = g_rnd.GetRandom(valDef._val,valDef._val2+1);
		break;
	case KValDef::val_getHp:
		ret = card->GetHp();
		break;
	default:
		ret = valDef.GetVal();
		break;
	}
	return ret;
}


KAbilityStatic* _findStaticAbility(int cardId,KAbilityStatic::Enum_When when)
{
	KCardAbilityList abilityList;
	KGameStaticMgr::getSingleton().GetAbilityList(cardId,abilityList,when);
	if(abilityList.empty()) return NULL;
	return *(abilityList.begin());
}

KAbilityStatic* _findStaticAbility(int cardId,KAbilityStatic::Enum_What what)
{
	KCardAbilityList abilityList;
	KGameStaticMgr::getSingleton().GetAbilityList(cardId,abilityList);
	for(KCardAbilityList::iterator it = abilityList.begin();it!=abilityList.end();++it){
		if((*it)->GetWhat()==what) return *it;
	}
	return NULL;
}

bool _needEnterFightTarget(int cardId)
{
	KAbilityStatic* pAbility = KSkillAssist::_findStaticAbility(cardId,KAbilityStatic::when_enter);

	if(!pAbility ||
		pAbility->IsTargetSure()||
		pAbility->IsArea()){
		return false;
	}else{
		return true;
	}
}

}
