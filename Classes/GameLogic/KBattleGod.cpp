#include "KBattleGod.h"
#ifdef _USE_COCOS2DX
#include "ccMacros.h"
#endif
#include "KBattleGuy.h"
#include "KCardInst.h"
#include "KBattleCtrlBase.h"
#include "KGameDef.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "../StaticTable/KAbilityStatic.h"
#include "assist/KSkillAssist.h"
#include "../Inc/KTypeDef.h"



template<> KBattleGod* Singleton<KBattleGod>::mSingleton = 0;

KBattleGod& KBattleGod::getSingleton(void)
{  
	if(!mSingleton){
		mSingleton = new KBattleGod;
		mSingleton->init();
	}
	return ( *mSingleton );  
}

void KBattleGod::init()
{
	m_TurnPlayer = NULL;
}

void KBattleGod::OnTurnBegin()
{
}

bool KBattleGod::ProcHandCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,int pos)
{
	CCLog("ProcHandCard=%s",pSrc->GetST()->GetName());
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	bool ret = false;
	switch(pSrc->GetType()){
	case KCardStatic::card_soldier:
		ret = DoCardToFightField(ctrl,pPlayer,pSrc,pDes,pos);
		break;
	case KCardStatic::card_skill:
		ret = DoUseSkillCard(ctrl,pPlayer,pSrc,pDes);
		break;
    case KCardStatic::card_secret:
        ret = DoUseSecretCard(ctrl,pPlayer,pSrc);
        break;
    default:
            break;
	}
	return ret;
}

bool KBattleGod::ProcEquipCard(KCardInst* pSrc,KCardInst* pDes)
{
	bool ret = false;
	return ret;
}

bool KBattleGod::ProcSecretCardAbility(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,KAbilityStatic::Enum_When when )
{
    KBattleGuy* pDefGuy = ctrl->GetDefGuy();
    KCardInst* pSecret = pDefGuy->GetDeck().GetSecret();
	if(!KSkillAssist::_checkSecretAbility(pSrc,pDes,pSecret,when)) return false;

	strCardAbilityResult result;
	result.init(pSrc->GetRealId(),pSecret->GetSecretAbility());
	DoCardAbility2Des(ctrl,pSecret->GetSecretAbility(),pSecret,pSrc,&result);
	SendAbilityResult(ctrl,result);
	pDefGuy->GetDeck().onSecret2Tomb(pSecret);
	return true;
}


bool KBattleGod::ProcCardDuel(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
{
	if(!pSrc->m_attr.getReady()){
		CCAssert(false , "Duel Card Need to be Ready!");
		return false;
	}
	if(pDes->IsHide()){
		CCAssert(false , "Defender can't be hiding!");
		return false;
	}
	if(!pDes->IsGuider()){
		KBattleGuy* pDefGuy = ctrl->GetDefGuy();
		KCardInstList enemyGuider;
		pDefGuy->GetDeck().FindFightingGuider(&enemyGuider);
		if(!enemyGuider.empty()){
			CCAssert(false , "Need to Duel Guider!");
			return false;
		}
	}
	if(ProcSecretCardAbility(ctrl, pSrc, pDes,KAbilityStatic::when_atked)) return true;
    
	bool ret = false;
	int atkSrc = pSrc->GetAtk();
	int atkDes = pDes->GetAtk();
	int v2 = (pSrc->IsDist())?0:pSrc->Heal(-atkDes);
	int v1 = pDes->Heal(-atkSrc);
	CCLog("ProcCardDuel=%s:%d,HP:%d->%d, VS %s:%d,HP:%d->%d",pSrc->GetST()->GetName(),pSrc->GetRealId(),pSrc->GetHp()-v2,pSrc->GetHp(),
													pDes->GetST()->GetName(),pDes->GetRealId(),pDes->GetHp()-v1,pDes->GetHp());

	pSrc->m_attr.setReady(0);
	SendDuelResult(ctrl,pSrc,pDes,v2,v1);
	PostCardDuel(ctrl,pSrc,pDes);
	return ret;
}

void KBattleGod::SendAbilityResult(KBattleCtrlBase* ctrl,strCardAbilityResult& result)
{
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_ABILITYRESULT,(unsigned long long)&result,(unsigned long long)ctrl->GetWorld());
}

void KBattleGod::SendDuelResult(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes,int v1,int v2)
{
	strCardDuelResult result;
	result._atker = pSrc;
	result._defender = pDes;
	result._val1 = v1;
	result._val2 = v2;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DUELRESULT,(unsigned long long)&result,(unsigned long long)ctrl->GetWorld());
}

void KBattleGod::PostCardDuel(KBattleCtrlBase* ctrl,KCardInst* pCard1,KCardInst* pCard2)
{
	if(pCard1&& pCard1->GetHp()<=0){
		ctrl->onCard2Tomb(pCard1);
	}
	if(pCard2&&pCard2->GetHp()<=0){
		ctrl->onCard2Tomb(pCard2);
	}
	if(pCard1&& pCard1->GetHp()<=0){
		DoCardAbilityOnWhen(ctrl,pCard1,KAbilityStatic::when_dead);
	}
	if(pCard2&&pCard2->GetHp()<=0){
		DoCardAbilityOnWhen(ctrl,pCard2,KAbilityStatic::when_dead);
	}
}

bool KBattleGod::DoCardAbilityOnWhen(KBattleCtrlBase* ctrl,KCardInst* card,KAbilityStatic::Enum_When when)
{
	KAbilityStatic* pAbility = card->FindBuf(when);
	if(pAbility){
		return DoCardAbility(ctrl,pAbility,card);
	}else{
		return false;
	}
}

bool KBattleGod::ProcHeroCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	strCardAbilityResult result;
	
	KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(pSrc->GetCardId()*10);
	if(!pAbility){
		CCAssert(false , "Not Hero ability!");
		return false;
	}
	if(!pSrc->m_attr.getReady()){
		CCAssert(false , "Hero's skill isn't ready!");
		return false;
	}
	if(pPlayer->GetCurRes()<pSrc->GetCost()){
		CCAssert(false , "Need more res!");
		return false;
	}
	if(pAbility->GetWhen()!=KAbilityStatic::when_use){
		return false;
	}
	DoCardAbility(ctrl,pAbility,pSrc,pDes);
	pPlayer->UseRes(pSrc->GetCost());
	pSrc->m_attr.setReady(0);
	return true;
}

bool KBattleGod::OnPlayCard(KBattleCtrlBase* ctrl,KBattleCtrlBase::BattleOp* op)
{
	bool ret = false;
	if(op->IsOK()){
		KCardInst* pSrc = ctrl->GetCard(op->_src);
		KCardInst* pDes = ctrl->GetCard(op->_des);
		if(!pSrc){
			CCAssert(false , "Not Found Init Card!");
			op->Empty();
			return ret;
		}
		switch(pSrc->GetSlot()){
		case KCardInst::enum_slot_hand:
			ret = ProcHandCard(ctrl,pSrc,pDes,op->_slot);
			break;
		case KCardInst::enum_slot_equip:
			ret = ProcEquipCard(pSrc,pDes);
			break;
		case KCardInst::enum_slot_fight:
			ret = ProcCardDuel(ctrl,pSrc,pDes);
			break;
		case KCardInst::enum_slot_hero:
			ret = ProcHeroCard(ctrl,pSrc,pDes);
			break;
        default:
            break;
		}
		op->Empty();
	}
	return ret;
}

void KBattleGod::OnTurnEnd()
{
}

void KBattleGod::AddRes(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	pPlayer->AddRes(pAbility->GetVal());
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_UPDATEINFO,0,(unsigned long long)ctrl->GetWorld());
}

void KBattleGod::CopyFight(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	KBattleGuy* pDef = ctrl->GetDefGuy();
	KCardInstList lst,newLst;
	pDef->GetDeck().RndPickCard(lst,1,KCardInst::enum_slot_slot,KCardStatic::card_soldier);
	pPlayer->GetDeck().CreateCloneCard(lst,newLst,KCardInst::enum_slot_fight);

	strCardAbilityResult result;
	result.init(pSrc->GetRealId(),pAbility);
	for(KCardInstList::iterator it = newLst.begin();it!=newLst.end();++it){
		result.SetDestVal((*it)->GetRealId(),0);
		KCardInst* card = ctrl->GetCard((*it)->GetRealId());
		pPlayer->GetDeck().Hand2Fight(card);
	}
	SendAbilityResult(ctrl,result);

}

void KBattleGod::CopyHand(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	KBattleGuy* pDef = ctrl->GetDefGuy();
	KCardInstList lst,newLst;
	pDef->GetDeck().RndPickCard(lst,pAbility->GetVal(),KCardInst::enum_slot_hand);
	pPlayer->GetDeck().CreateCloneCard(lst,newLst,KCardInst::enum_slot_hand);

	strCardAbilityResult result;
	result.init(pSrc->GetRealId(),pAbility);
	for(KCardInstList::iterator it = newLst.begin();it!=newLst.end();++it){
		result.SetDestVal((*it)->GetRealId(),0);
	}
	SendAbilityResult(ctrl,result);
}

void KBattleGod::DrawCard(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	pPlayer->GetDeck().DrawCard(pAbility->GetVal());
}

void KBattleGod::SummonCard(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility)
{
	KBattleGuy* pPlayer = ctrl->GetCurGuy();
	strSummonCardResult result;
	result._src = pSrc;
	result._des = pPlayer->GetDeck().SummonCard(pAbility->GetVal());
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_SUMMONCARD,(unsigned long long)&result,(unsigned long long)ctrl->GetWorld());
}

bool KBattleGod::DoCardAbility(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes)
{
	if(pSrc->GetOwner()->DoGuyAbility(pAbility)) return true;
	if(pDes && pDes->HasBuf(pAbility)) return false;

	if(pAbility->GetWhat()==KAbilityStatic::what_res_add){
		AddRes(ctrl,pAbility);
		return true;
	}else if(pAbility->GetWhat()==KAbilityStatic::what_copy_hand){
		CopyHand(ctrl,pSrc,pAbility);
		return true;
	}else if(pAbility->GetWhat()==KAbilityStatic::what_copy_fight){
		if(pSrc->GetOwner()->GetDeck().GetEmptyFightSlot()<0) return false;
		CopyFight(ctrl,pSrc,pAbility);
		return true;
	}else if(pAbility->GetWhat()==KAbilityStatic::what_draw_card){
		DrawCard(ctrl,pAbility);
		return true;
	}else if(pAbility->GetWhat()==KAbilityStatic::what_summon){
		SummonCard(ctrl,pSrc,pAbility);
		return true;
	}else if(pAbility->GetWhat()==KAbilityStatic::what_sp_rate){
	}
	bool ret = false;
	strCardAbilityResult result;
	result.init(pSrc->GetRealId(),pAbility);
	if(!pAbility->IsArea()&&pDes){
		KCardInstList lst;
		KSkillAssist::_fillAbilityTarget(ctrl,pSrc,pAbility,&lst);
		int pos = _getIndexOfCard(&lst,pDes);
		if(pos>=0){
			DoCardAbility2Des(ctrl,pAbility,pSrc,pDes,&result);
			ret =true;
		}
	}else{
		KCardInstList lst;
		KSkillAssist::_fillAbilityTarget(ctrl,pSrc,pAbility,&lst);
		KSkillAssist::_rndFillProc(pAbility,&lst);

		ret = (lst.size()>0)?true:false;
		for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
			DoCardAbility2Des(ctrl,pAbility,pSrc,*it,&result);
		}
	}
	if(ret) SendAbilityResult(ctrl,result);
	return ret;
}

void KBattleGod::DoCardAbility2Des(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes,strCardAbilityResult* result)
{
	KBattleGuy* guy = pSrc->GetOwner();
	KBattleDeck* deck = ctrl->GetCardDeck(pDes);
	if(!pAbility->BufIconEmpty()) pDes->AddBuf(pAbility);
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_damage:
		{
			int val = guy->calcHurtVal(pAbility->GetVal());
			val = pDes->Heal(-val);
			PostCardDuel(ctrl,pDes,NULL);
			result->SetDestVal(pDes->GetRealId(),val);
			CCLog("Skill:%s:%d do damage:%d to:%s:%d",pSrc->GetST()->GetName(),pSrc->GetRealId(),val,pDes->GetST()->GetName(),pDes->GetRealId());
		}
		break;
	case KAbilityStatic::what_heal:
		{
			int val = guy->calcHealVal(pAbility->GetVal());
			val = pDes->Heal(val);
			result->SetDestVal(pDes->GetRealId(),val);
			CCLog("Skill:%s do heal:%d to:%s",pSrc->GetST()->GetName(),val,pDes->GetST()->GetName());
		}
		break;
	case KAbilityStatic::what_immune:
		{
			result->SetDestVal(pDes->GetRealId(),0);
			CCLog("%s do immune to:%s,id=%d",pSrc->GetST()->GetName(),pDes->GetST()->GetName(),pDes->GetRealId());
		}
		break;
	case KAbilityStatic::what_replace:
		{
			pDes->ReplaceST(pAbility->GetVal());
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_return:
		{
			deck->Fight2Hand(pDes);
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_atk_add:
		{
			pDes->AddAtk(pAbility->GetVal());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetVal());
		}
		break;
	case KAbilityStatic::what_clear_buf:
		{
			pDes->ClearBuf();
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_hp_set:
		{
			pDes->HpSet(pAbility->GetVal());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetVal());
		}
		break;
	case KAbilityStatic::what_guide:
		{
			//TBD
		}
		break;
	case KAbilityStatic::what_kill:
	case KAbilityStatic::what_kill_atk_le:
	case KAbilityStatic::what_kill_atk_he:
		{
			ctrl->onCard2Tomb(pDes);
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_hp_double:
		{
			pDes->HpDouble();
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_hp_add:
		{
			pDes->AddHp(pAbility->GetVal());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetVal());
		}
		break;
	case KAbilityStatic::what_control:
		{
			ctrl->onCardSwitchOwner(pSrc,pDes);
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
    default:
        break;
	}
}

bool KBattleGod::DoUseSkillCard(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pSrc,KCardInst* pDes)
{
	if(pSrc->GetCost()>guy->GetCurRes()) return false;

    
	KCardAbilityList abilityLst;
	KGameStaticMgr::getSingleton().GetAbilityList(pSrc->GetCardId(),abilityLst,KAbilityStatic::when_use);

	pSrc->m_attr.setSlot(KCardInst::enum_slot_fight);
	
	for(KCardAbilityList::iterator it=abilityLst.begin();it!=abilityLst.end();++it){
		strCardAbilityResult result;
		KAbilityStatic* pAbility = *it;
		DoCardAbility(ctrl,pAbility,pSrc,pDes);
	}
	guy->UseRes(pSrc->GetCost());
	ctrl->onCard2Tomb(pSrc);
	return true;
}

bool KBattleGod::DoCardToSecretField(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pCard)
{
	if(!pCard){
		CCAssert(false , "Not found Card!");
		return false;
	}
	if(guy->GetCurRes() < pCard->GetCost()){
		CCAssert(false , "Not Enough Res!");
		return false;
	}
	guy->UseRes(pCard->GetCost());
	guy->GetDeck().Hand2Secret(pCard);
	return true;
}

bool KBattleGod::DoCardToFightField(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pCard,KCardInst* pDes,int pos)
{
	if(!pCard){
		CCAssert(false , "Not found Card!");
		return false;
	}
	if(guy->GetCurRes() < pCard->GetCost()){
		CCAssert(false , "Not Enough Res!");
		return false;
	}
	if(pos<0) pos = guy->GetDeck().GetEmptyFightSlot();

    if(pos<0){
        CCAssert(false , "Not Empty Pos!");
		return false;
    }
	guy->UseRes(pCard->GetCost());
	guy->GetDeck().Hand2Fight(pCard,pos);
	KAbilityStatic* pAbility = pCard->FindBuf(KAbilityStatic::when_enter);
	bool doAbilityOk = false;
	if(pAbility){
		if(pDes){
			DoCardAbility(ctrl,pAbility,pCard,pDes);
			doAbilityOk = true;
		}else{
			doAbilityOk = DoCardAbilityOnWhen(ctrl,pCard,KAbilityStatic::when_enter);
		}
	}
	
	if(!doAbilityOk || !pAbility ||pAbility->ActionIsEmpty())
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_CARDMOVE,pCard->GetRealId(),(unsigned long long)ctrl->GetWorld());
	return true;
}

bool KBattleGod::DoUseSecretCard(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pSrc)
{
    if(guy->GetDeck().GetSecret()) return false;
    return DoCardToSecretField(ctrl,guy,pSrc);
}
