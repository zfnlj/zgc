#include "KCardInst.h"
#include "../StaticTable/KAbilityStatic.h"
#include "../StaticTable/KGameStatic.h"
#include "KBattleGod.h"
#include "System/Misc/KStream.h"
#include "assist/KBattleEventAssist.h"
#include "assist/KSkillAssist.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"

void _removeFromCardList(KCardInstList& lst,KCardInst* card)
{
	KCardInstList::iterator it = _findCardIt(&lst,card);
	if(it!=lst.end()) lst.erase(it);
}

void _replaceFromCardList(KCardInstList& lst,KCardInst* card,KCardInst* newcard)
{
	KCardInstList::iterator it = _findCardIt(&lst,card);
	if(it!=lst.end()){
		*it = newcard;
	}
}

void _rndPickCard(KCardInstList& src,KCardInstList& des,int num,KCardStatic::CardDef cardDef)
{
	KCardInstList tmpLst;
	for(KCardInstList::iterator it=src.begin();it!=src.end();it++){
		if(cardDef==KCardStatic::card_null ||
			(*it)->IsKindOf(cardDef))
			tmpLst.push_back(*it);
	}

	for(int i=0;i<num;i++){
		int nRand = g_rnd.GetRandom(0,tmpLst.size());
		int pos=0;
		for(KCardInstList::iterator it=tmpLst.begin();it!=tmpLst.end();it++,pos++){
			if(nRand==pos){
				des.push_back(*it);
				tmpLst.erase(it);
				break;
			}
		}
	}
}

void _fillDirtyCardList(KCardInstList& src,KCardInstList& des)
{
	for(KCardInstList::iterator it = src.begin();it!=src.end();++it){
		if((*it)->IsDirty()) des.push_back(*it);
	}
}
int _getDirtyCardCount(KCardInstList* lst)
{
	int n = 0;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		if((*it)->IsDirty()) n++;
	}
	return n;
}
int _getIndexOfCard(KCardInstList* lst,KCardInst* card)
{
	int index = 0;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it,++index){
		if(*it==card) return index;
	}
	return -1;
}

void _copyCardSet(KCardInstList* src,KCardInstList* des,KCardInst* skip,KCardInst::CardSlot slot)
{
	for(KCardInstList::iterator it = src->begin(); it!=src->end();++it){
		if(slot!=KCardInst::enum_slot_null && (*it)->GetSlot()!=slot) continue;
		if((*it)==skip) continue;
		des->push_back(*it);
	}
}

KCardInstList::iterator _findCardIt(KCardInstList* lst,KCardInst* card)
{
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();it++){
		if((*it)->GetRealId() == card->GetRealId()) return it;
	}
	return lst->end();
}

void _clearCardList(KCardInstList* lst)
{
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();it++){
		KCardInst::Free((*it));
	}
	lst->clear();
}

bool KCardInst::init()
{
	m_Owner = NULL;
	m_attr.clearAttrs();
	m_pST = NULL;
#ifdef _USE_COCOS2DX
	m_actor = NULL;
#endif
	m_bClone = false;
	return true;
}

void KCardInst::clone(KCardInst* card)
{
	m_attr.clone(&card->m_attr);
	m_pST = card->m_pST;
	m_attr.clearBit();
	m_actor = card->m_actor;
	m_Owner = card->m_Owner;
	m_bClone = true;
}

KCardInst::~KCardInst()
{
	
}


KCardInst* KCardInst::create()
{
	KCardInst* card = KCardInst::Alloc();
	card->init();
	return card;
}

void KCardInst::Clear()
{
#ifdef _USE_COCOS2DX
	releaseActor();
#endif
}

void KCardInst::init(int realId,KBattleGuy* owner,int cardId)
{
	KCardStatic* st = KGameStaticMgr::getSingleton().GetCard(cardId);
	init();
	m_Owner = owner;
	m_pST = st;
	m_attr.init(realId,st);
}

int KCardInst::GetCost()
{ 
	return m_pST->GetCost();
}

#ifdef _USE_COCOS2DX
void KCardInst::retainActor(CCObject* actor)
{
	if(m_actor==actor) return;
	m_actor = actor;
	CC_SAFE_RETAIN(m_actor);
}

void KCardInst::releaseActor()
{
	CC_SAFE_RELEASE(m_actor);
	m_actor = NULL;
}

#endif

void KCardInst::EnterSecretField(int pos)
{
	m_attr.setPos(pos);
	m_attr.setSlot(KCardInst::enum_slot_secret);
}

void KCardInst::onSwitchFightField(int pos)
{
	m_attr.setPos(pos);
	m_attr.setReady(0);
	m_attr.setSlot(KCardInst::enum_slot_fight);
}

void KCardInst::EnterFightField(int pos)
{
    m_attr.setPos(pos);
    m_attr.setSlot(KCardInst::enum_slot_fight);
	m_attr.setReady(0);
	KAttrStatic* pAttr = KGameStaticMgr::getSingleton().GetAttr(m_pST->GetID());
	bool rushed = false;
	if(pAttr){
		if(pAttr->IsRush()){
			m_attr.setReady(1);
			rushed = true;
		}
		if(pAttr->IsGuide()) AddBuf(BUF_GUIDE_ID);
		if(pAttr->IsHide()) AddBuf(BUF_HIDE_ID);
		if(pAttr->IsDist()) AddBuf(BUF_DIST_ID);
	}
	if(!rushed){
		KAbilityStatic* pNoReady = KSkillAssist::_findStaticAbility(GetCardId(),KAbilityStatic::what_noready);
		if(!pNoReady) AddBuf(BUF_CAN_RUSH_ID);
	}

}

void KCardInst::onTurnBegin(KBattleCtrlBase* ctrl)
{
	switch(GetSlot()){
	case enum_slot_fight:
		{
			KAbilityStatic* pNoReady = KSkillAssist::_findStaticAbility(GetCardId(),KAbilityStatic::what_noready);
			if(!FindBuf(KAbilityStatic::what_stun) && !pNoReady){
				m_attr.setReady(1);
			}
			m_attr.updateBufList();
			if(!FindBuf(KAbilityStatic::what_stun)) onCardAbility(ctrl,KAbilityStatic::when_turn_begin);
		}
		break;
	case enum_slot_secret:
		{
			AddHp(1);
		}
		break;
	case enum_slot_hero:
		{
			m_attr.setReady(1);
		}
		break;
	default:
		break;
	} 
}

void KCardInst::OnTurnEnd(KBattleCtrlBase* ctrl)
{
	if(!FindBuf(KAbilityStatic::what_stun)) onCardAbility(ctrl,KAbilityStatic::when_turn_end);
	m_attr.updateBufList();
}

void KCardInst::HpDouble()
{
	int hp = GetHp()*2;
	m_attr.setCurHp(hp);
	m_attr.setMaxHp(hp);
}

void KCardInst::ReturnHand()
{
	ClearBuf();
	int realId = m_attr.getRealID();
	m_attr.init(realId,m_pST);
	m_attr.setSlot(KCardInst::enum_slot_hand);
}

void KCardInst::HpSet(int val)
{
	m_attr.setCurHp(val);
	m_attr.setMaxHp(val);
}

void KCardInst::CurHpSet(int cur)
{
	m_attr.setCurHp(cur);
}

void KCardInst::AddHp(int val)
{
	m_attr.setCurHp(m_attr.getCurHp()+val);
	m_attr.setMaxHp(m_attr.getMaxHp()+val);
}

float KCardInst::GetAtkedVal(int damage)
{
	KAbilityStatic* pEmmune = FindBuf(KAbilityStatic::what_immune);
	if(pEmmune) return GetAtk();
	float hp = (float)m_attr.getCurHp();
	if(hp>damage) return ((float)damage)*0.4f;
	return hp*0.4f +GetAtk()*0.5f;
}

int KCardInst::Heal(KCardInst* pSrc,int val)
{
	KAbilityStatic* pEmmune = FindBuf(KAbilityStatic::what_immune);
	if(val < 0 && pEmmune){ //有免疫buf ，优先使用buf.
		DelBuf(pEmmune);
		return 0;
	}
	int hp = m_attr.getCurHp();
	hp += val;
	if(hp > m_attr.getMaxHp()) hp = m_attr.getMaxHp();
	if(hp<0) hp = 0;
	KAbilityStatic* pBlessHp = FindBuf(KAbilityStatic::what_bless_hp);
	if(pBlessHp){
		if(hp < pBlessHp->GetNormalVal()) hp = pBlessHp->GetNormalVal();
	}
	int ret = hp - m_attr.getCurHp();
	m_attr.setCurHp(hp);
	if(ret<0){
		if(!m_bClone) KBattleEvtAssist::_onBattleEvt(battle_evt_hurted,m_Owner->GetBattleCtrl(),pSrc,this);
		AddBuf(BUF_HURTED_ID);
	}else if(ret>0){
		if(!m_bClone) KBattleEvtAssist::_onBattleEvt(battle_evt_healed,m_Owner->GetBattleCtrl(),pSrc,this);
	}

	return ret;
}

void KCardInst::AddAtk(int val)
{
	m_attr.setAddAtk(m_attr.getAddAtk()+val);
}

int KCardInst::GetHp()
{
	return m_attr.getCurHp();
}

void KCardInst::AddBuf(int id)
{
	KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(id);
	if(pBuf) m_attr.AddBuf(pBuf,pBuf->LoopNum());
}

void KCardInst::AddBuf(KAbilityStatic* pBuf,int loopNum)
{
	m_attr.AddBuf(pBuf,loopNum);
}

void KCardInst::DelBuf(KAbilityStatic* pBuf)
{
	m_attr.DelBuf(pBuf);
}

int KCardInst::CalcGoodBufVal()
{
	int ret = 0;
	KCardBufferList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhen()!=KAbilityStatic::when_ever&&
			buf._pST->GetAbilityType()==KAbilityStatic::ability_good){
			ret += buf._pST->GetBufVal();
		}
		it++;
	}
	return ret;
}

void KCardInst::DispleGoodBuf()
{
	KCardBufferList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhen()==KAbilityStatic::when_ever||
			buf._pST->GetAbilityType()==KAbilityStatic::ability_bad||
			buf._pST->GetAbilityType()==KAbilityStatic::ability_neutral){
			it++;
		}else{
			if(buf._pST->GetWhich()==KAbilityStatic::which_owner) m_Owner->RemoveGuyAbility(buf._pST);
			it = m_attr.m_bufList.erase(it);
		}
	}
}

void KCardInst::DispleBuf()
{
	KCardBufferList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhen()==KAbilityStatic::when_ever){
			it++;
		}else{
			if(buf._pST->GetWhich()==KAbilityStatic::which_owner) m_Owner->RemoveGuyAbility(buf._pST);
			if(buf._pST->GetWhat()==KAbilityStatic::what_atk_equ_hp) m_attr.setAddAtk(GetHp());
			it = m_attr.m_bufList.erase(it);
		}
	}
}

void KCardInst::ClearBuf()
{
	KCardBufferList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		KAbilityStatic* pBuf = (*it)._pST;
		if(pBuf->GetWhich()==KAbilityStatic::which_owner){
			m_Owner->RemoveGuyAbility(pBuf);
		}
		it++;
	}
	m_attr.clearBuf();
}

bool KCardInst::HasBuf(KAbilityStatic* pBuf)
{
	return m_attr.HasBuf(pBuf);
}

KAbilityStatic* KCardInst::FindBuf(KAbilityStatic::Enum_What what)
{
	return m_attr.FindBuf(what);
}

//KAbilityStatic* KCardInst::FindBufAbility(KAbilityStatic::Enum_What what)
//{
//	return m_attr.FindBufAbility(what);
//}

KAbilityStatic* KCardInst::FindBuf(KAbilityStatic::Enum_When when)
{
	return m_attr.FindBuf(when);
}

void KCardInst::onCard2Tomb()
{
	ClearBuf();
	m_attr.setSlot(KCardInst::enum_slot_tomb);
}

void KCardInst::onCardAbility(KBattleCtrlBase* ctrl,KAbilityStatic::Enum_When when)
{
	KAbilityStatic* pAbility = FindBuf(when);
	if(!pAbility && m_pST->IsRare()){
		KCardAbilityList abilityLst;
		KGameStaticMgr::getSingleton().GetAbilityList(GetCardId(),abilityLst,when);
		if(abilityLst.size()>0)
			pAbility = abilityLst.front();
	}
	if(pAbility){
		KBattleGod::getSingleton().DoCardAbility(ctrl,pAbility,this);
	}
}

void KCardInst::ReplaceST(int id)
{
	ClearBuf();
	int realId = m_attr.getRealID();
	m_pST = KGameStaticMgr::getSingleton().GetCard(id);
	int slot = m_attr.getSlot();
	int pos = m_attr.getPos();
	m_attr.init(realId,m_pST);
	m_attr.setSlot(slot);
	m_attr.setPos(pos);
}

const char* KCardInst::GetBasePosName(bool bMy)
{
	static char sz[64];
	switch(GetSlot()){
	case KCardInst::enum_slot_select:
		return ((bMy))?"select_base":"your_hand_base";
		break;
	case KCardInst::enum_slot_hand:
		return (bMy)? "my_hand_base":"your_hand_base";
		break;
	case KCardInst::enum_slot_fight:
		{
			sprintf(sz,"%s_%d",(bMy)? "my_fight_slot":"your_fight_slot",m_attr.getPos());
		}
		return sz;
		break;
	case KCardInst::enum_slot_hero:
		return (bMy)? "my_hero_base":"your_hero_base";
		break;
	case KCardInst::enum_slot_tomb:
		return (bMy)? "my_tomb_base":"your_tomb_base";
		break;
	case KCardInst::enum_slot_slot:
		return (bMy)? "my_slot_base":"your_slot_base";
		break;
    case KCardInst::enum_slot_secret:
		{
			sprintf(sz,"%s_%d",(bMy)? "my_secret_slot":"your_secret_slot",m_attr.getPos());
		}
		return sz;
        break;
	default:
		ASSERT(false);//CCAssert(false , "TBD!");
		break;
	}
	return "";
}

bool KCardInst::IsActiveDefend()
{
	switch(GetKind()){
	case KCardStatic::card_soldier:
		{
			if(FindBuf(KAbilityStatic::what_hide)) return false;	
		}
		break;
	case KCardStatic::card_hero:
		{
			return true;
		}
	default:
		return false;
	}
	
	return true;
}

bool KCardInst::IsTargetLess(KAbilityStatic::Enum_When when)
{
	KAbilityStatic* pAbility = KSkillAssist::_findStaticAbility(GetCardId(),when);
	if(!pAbility) return false;
	if(pAbility->IsArea()) return true;
	if(pAbility->GetWhich()==KAbilityStatic::which_i ||
		pAbility->GetWhich()==KAbilityStatic::which_u||
		pAbility->GetWhich()==KAbilityStatic::which_owner||
		pAbility->GetWhich()==KAbilityStatic::which_myhero||
		pAbility->GetWhich()==KAbilityStatic::which_yourhero)
		return true;
	return false;
}

bool KCardInst::IsRangeAbility(KAbilityStatic::Enum_When when)
{
	KAbilityStatic* pAbility = FindBuf(when);
	if(!pAbility) return false;
	return pAbility->IsArea();
}


size_t KCardInst::serializeDirty(KMemoryStream* so)
{
	size_t pos = so->size();
	m_attr.updateMask(KCardAttr::REAL_ID);
	m_attr.updateMask(KCardAttr::CARD_ID);
	m_attr.writePacketFilter(so,0,true);
	return so->size() - pos;
}

size_t KCardInst::serialize(KMemoryStream* so)
{
	size_t pos = so->size();

	m_attr.writePacketAll(so,true);

	return so->size() - pos;
}

BOOL KCardInst::deserialize(KMemoryStream* si)
{
	m_attr.readPacket(si);
	m_pST = KGameStaticMgr::getSingleton().GetCard(m_attr.getCardId());
	return TRUE;
}

KAbilityStatic* KCardInst::GetSecretAbility()
{
	return KGameStaticMgr::getSingleton().GetAbilityOnId(GetCardId()*10);
}

void KCardInst::AtkSet(int val)
{
	m_attr.setAddAtk(val-m_pST->GetAtk());
}

int KCardInst::GetAtk()
{
	if(FindBuf(KAbilityStatic::what_atk_equ_hp)){
		return GetHp();
	}else{
		return m_pST->GetAtk() + m_attr.getAddAtk();
	} 
}

int KCardInst::GetRealCost()
{
	if(GetKind()==KCardStatic::card_skill|| GetKind()==KCardStatic::card_hero)
	{
		return m_Owner->calcMpCost(GetCost());
	}else{
		return GetCost();
	}
}

bool KCardInst::IsDead()
{
	return GetHp()<=0;
}

void KCardInst::DoRush()
{
	if(!FindBuf(KAbilityStatic::what_can_rush)) return;
	m_attr.DelBuf(KAbilityStatic::what_can_rush);
	m_attr.setReady(1);
}