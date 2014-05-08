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
#include "assist/KBattleEventAssist.h"


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
	switch(pSrc->GetKind()){
	case KCardStatic::card_soldier:
		ret = DoCardToFightField(ctrl,pPlayer,pSrc,pDes,pos);
		break;
	case KCardStatic::card_skill:
		ret = DoUseSkillCard(ctrl,pPlayer,pSrc,pDes);
		break;
    case KCardStatic::card_secret:
        ret = DoCardToSecretField(ctrl,pPlayer,pSrc);
        break;
    default:
            break;
	}
	if(ret) KBattleEvtAssist::_onBattleEvt(battle_evt_use_card,ctrl,pSrc,pDes);
	return ret;
}

bool KBattleGod::ProcEquipCard(KCardInst* pSrc,KCardInst* pDes)
{
	bool ret = false;
	return ret;
}

bool KBattleGod::ProcSecretCardAbility(KBattleCtrlBase* ctrl,KCardInst** pSrc,KCardInst** pDes,KAbilityStatic::Enum_When when )
{
    KCardInst* pSecret = KSkillAssist::_findActiveSecret(ctrl,*pSrc,*pDes,when );
	if(!pSecret) return false;
	return KSkillAssist::_doSecretAbility(ctrl,pSecret,pSrc,pDes);
}


bool KBattleGod::ProcCardDuel(KBattleCtrlBase* ctrl,KCardInst* pSrc,KCardInst* pDes)
{
	if(!pSrc->m_attr.getReady()){
		CCAssert(false , "Duel Card Need to be Ready!");
		return false;
	}
	if(pDes->FindBuf(KAbilityStatic::what_hide)){
		CCAssert(false , "Defender can't be hiding!");
		return false;
	}
	if(!pDes->FindBuf(KAbilityStatic::what_guide)){
		KBattleGuy* pDefGuy = ctrl->GetDefGuy();
		KCardInstList enemyGuider;
		pDefGuy->GetDeck().FindFightingGuider(&enemyGuider);
		if(!enemyGuider.empty()){
			CCAssert(false , "Need to Duel Guider!");
			return false;
		}
	}
	if(ProcSecretCardAbility(ctrl, &pSrc, &pDes,KAbilityStatic::when_atked)) return true;
    
	bool ret = false;
	int atkSrc = pSrc->GetAtk();
	int atkDes = pDes->GetAtk();
	int v2 = (pSrc->FindBuf(KAbilityStatic::what_dist))?0:pSrc->Heal(pDes,-atkDes);
	int v1 = pDes->Heal(pSrc,-atkSrc);
	CCLog("ProcCardDuel=%s:%d,HP:%d->%d, VS %s:%d,HP:%d->%d",pSrc->GetST()->GetName(),pSrc->GetRealId(),pSrc->GetHp()-v2,pSrc->GetHp(),
													pDes->GetST()->GetName(),pDes->GetRealId(),pDes->GetHp()-v1,pDes->GetHp());

	pSrc->m_attr.setReady(0);
	if(pSrc->IsDead()) pSrc->m_attr.setSlot(KCardInst::enum_slot_tomb);
	if(pDes->IsDead()) pDes->m_attr.setSlot(KCardInst::enum_slot_tomb);

	SendDuelResult(ctrl,pSrc,pDes,v2,v1);
	PostCardDuel(ctrl,pSrc,v2,pDes,v1);
	ctrl->AddDramaElapsed(4.0f);
	return ret;
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

void KBattleGod::PostCardDuel(KBattleCtrlBase* ctrl,KCardInst* pCard1,int val1,KCardInst* pCard2,int val2)
{
	if(pCard1 && pCard2){
		KAbilityStatic* pAbility = pCard2->FindBuf(KAbilityStatic::when_do_damage);
		if(pAbility && !pCard1->IsDead() && val1<0){
			DoCardAbility(ctrl,pAbility,pCard2,pCard1);
		}
		pAbility = pCard1->FindBuf(KAbilityStatic::when_do_damage);
		if(pAbility && !pCard2->IsDead() && val2<0){
			DoCardAbility(ctrl,pAbility,pCard1,pCard2);
		}

	}
	if(pCard1&& pCard1->IsDead()){
		KBattleEvtAssist::_onBattleEvt(battle_evt_duel_dead,ctrl,pCard2,pCard1);
		ctrl->onCard2Tomb(pCard1);
	}
	if(pCard2&&pCard2->IsDead()){
		KBattleEvtAssist::_onBattleEvt(battle_evt_duel_dead,ctrl,pCard1,pCard2);
		ctrl->onCard2Tomb(pCard2);
	}
}

bool KBattleGod::DoCardAbilityOnWhen(KBattleCtrlBase* ctrl,KCardInst* card,KAbilityStatic::Enum_When when,int actor)
{
	KAbilityStatic* pAbility = (when==KAbilityStatic::when_enter)? KSkillAssist::_findStaticAbility(card->GetCardId(),when) :card->FindBuf(when);
	if(pAbility){
		return DoCardAbility(ctrl,pAbility,card,NULL,actor);
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
	if(pPlayer->GetCurRes()<pSrc->GetRealCost()){
		CCAssert(false , "Need more res!");
		return false;
	}
	if(pAbility->GetWhen()!=KAbilityStatic::when_use){
		return false;
	}
	DoCardAbility(ctrl,pAbility,pSrc,pDes);
	pPlayer->UseRes(pSrc->GetRealCost());
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
		ctrl->DoCardEvtList(pSrc);
		op->Empty();
	}
	return ret;
}

void KBattleGod::OnTurnEnd()
{
}

bool KBattleGod::DoCardAbility(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility,KCardInst* pSrc,KCardInst* pDes,int actor)
{
	if(pAbility->GetWhich()==KAbilityStatic::which_owner) return pSrc->GetOwner()->DoGuyAbility(ctrl,pSrc,pAbility,actor);
	if(pDes && pDes->HasBuf(pAbility)) return false;

	bool ret = false;
	if(actor==0) actor=pSrc->GetRealId();
	strCardAbilityResult result;

	result.init(actor,pSrc->GetRealId(),pAbility);
	if(pAbility->IsArea() ||
		pAbility->IsTargetSure()){
		KCardInstList lst;
		KSkillAssist::_fillAbilityTarget(ctrl,pSrc,pDes,pAbility,&lst);
		KSkillAssist::_rndFillProc(ctrl,pSrc,pAbility,&lst);
		for(KCardInstList::iterator it = lst.begin();it!=lst.end();++it){
			DoCardAbility2Des(ctrl,pAbility,pSrc,*it,&result);
			ret = true;
		}
	}else if(pDes){
		KCardInstList lst;
		KSkillAssist::_fillAbilityTarget(ctrl,pSrc,pDes,pAbility,&lst);
		int pos = _getIndexOfCard(&lst,pDes);
		if(pos>=0){
			DoCardAbility2Des(ctrl,pAbility,pSrc,pDes,&result);
			ret =true;
		}
	}
	if(ret) KSkillAssist::_sendAbilityResult(ctrl,result);
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
			int damageVal = KSkillAssist::_calcValDef(ctrl,guy,pSrc,pAbility->GetVal());
			int val = guy->calcHurtVal(damageVal);
			val = pDes->Heal(pSrc,-val);
			PostCardDuel(ctrl,pDes,val,NULL,0);
			result->SetDestVal(pDes->GetRealId(),val);
			CCLog("Skill:%s:%d do damage:%d to:%s:%d",pSrc->GetST()->GetName(),pSrc->GetRealId(),val,pDes->GetST()->GetName(),pDes->GetRealId());
		}
		break;
	case KAbilityStatic::what_add_atk_hp:
		{
			pDes->AddAtk(pAbility->GetNormalVal());
			pDes->AddHp(pAbility->GetVal2());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetNormalVal());
		}
		break;
	case KAbilityStatic::what_heal:
		{
			int val = guy->calcHealVal(pAbility->GetNormalVal());
			val = pDes->Heal(pSrc,val);
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
			pDes->ReplaceST(pAbility->GetNormalVal());
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
			pDes->AddAtk(pAbility->GetNormalVal());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetNormalVal());
		}
		break;
	case KAbilityStatic::what_angry:
		{
			pDes->AddAtk(pAbility->GetNormalVal());
			pDes->m_attr.DelBuf(pAbility->GetVal2());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetNormalVal());
		}
		break;
	case KAbilityStatic::what_damage_atkadd:
		{
			pDes->Heal(pSrc,-pAbility->GetNormalVal());
			pDes->AddAtk(pAbility->GetVal2());
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_dispel_buf:
		{
			pDes->DispleBuf();
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_atkhp_sw:
		{
			int newHp = pDes->GetAtk();
			int newAtk = pDes->GetHp();
			pDes->HpSet(newHp);
			pDes->AtkSet(newAtk);
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_hp_set:
		{
			int newHp = KSkillAssist::_calcValDef(ctrl,guy,pDes,pAbility->GetVal());
			pDes->HpSet(newHp);
			result->SetDestVal(pDes->GetRealId(),newHp);
		}
		break;
	case KAbilityStatic::what_atk_set:
		{
			int newAtk = KSkillAssist::_calcValDef(ctrl,guy,pDes,pAbility->GetVal());
			pDes->AtkSet(newAtk);
			result->SetDestVal(pDes->GetRealId(),newAtk);
		}
		break;
	case KAbilityStatic::what_kill:
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
			pDes->AddHp(pAbility->GetNormalVal());
			result->SetDestVal(pDes->GetRealId(),pAbility->GetNormalVal());
		}
		break;
	case KAbilityStatic::what_control:
		{
			ctrl->onCardSwitchOwner(pSrc,pDes);
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
	case KAbilityStatic::what_rush:
		{
			pDes->DoRush();
			result->SetDestVal(pDes->GetRealId(),0);
		}
		break;
    default:
		result->SetDestVal(pDes->GetRealId(),0);
        break;
	}
}

bool KBattleGod::DoUseSkillCard(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pSrc,KCardInst* pDes)
{
	if(pSrc->GetRealCost()>guy->GetCurRes()) return false;

    
	KCardAbilityList abilityLst;
	KGameStaticMgr::getSingleton().GetAbilityList(pSrc->GetCardId(),abilityLst,KAbilityStatic::when_use);

	pSrc->m_attr.setSlot(KCardInst::enum_slot_fight);
	
	KBattleEvtAssist::_onBattleEvt(battle_evt_use_skill,ctrl,pSrc,pDes);

	if(ProcSecretCardAbility(ctrl, &pSrc, &pDes,KAbilityStatic::when_use_skill)) return true;

	bool ret=false;
	for(KCardAbilityList::iterator it=abilityLst.begin();it!=abilityLst.end();++it){
		strCardAbilityResult result;
		KAbilityStatic* pAbility = *it;
		if(DoCardAbility(ctrl,pAbility,pSrc,pDes)) ret =true;
	}
	if(!ret){
		strCardAbilityResult result;
		result.init(pSrc->GetRealId(),pSrc->GetRealId(),*abilityLst.begin());
		KSkillAssist::_sendAbilityResult(ctrl,result);
	}
	guy->UseRes(pSrc->GetRealCost());
	ctrl->onCard2Tomb(pSrc);
	ctrl->AddDramaElapsed(6.0f);
	return true;
}

bool KBattleGod::DoCardToSecretField(KBattleCtrlBase* ctrl,KBattleGuy* guy,KCardInst* pCard)
{
	int pos = guy->GetDeck().GetEmptySecretSlot();
	if(pos<0){
		CCAssert(false , "secret set is full!");
		return false;
	}
	if(!pCard){
		CCAssert(false , "Not found Card!");
		return false;
	}
	if(guy->GetCurRes() < pCard->GetCost()){
		CCAssert(false , "Not Enough Res!");
		return false;
	}
	guy->UseRes(pCard->GetCost());
	guy->GetDeck().Hand2Secret(pCard,pos);
	ctrl->AddDramaElapsed(3.0f);
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
	
	KAbilityStatic* pAbility = KSkillAssist::_findStaticAbility(pCard->GetCardId(),KAbilityStatic::when_enter);
	bool doAbilityOk = false;
	if(pAbility){
		if(pDes){
			DoCardAbility(ctrl,pAbility,pCard,pDes);
			doAbilityOk = true;
		}else{
			doAbilityOk = DoCardAbilityOnWhen(ctrl,pCard,KAbilityStatic::when_enter);
		}
	}
	
	if(!doAbilityOk || !pAbility ||pAbility->ActionIsEmpty()){
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_CARDMOVE,pCard->GetRealId(),(unsigned long long)ctrl->GetWorld());
		ctrl->AddDramaElapsed(2.0f);
	}else{
		ctrl->AddDramaElapsed(4.0f);
	}
	return true;
}


