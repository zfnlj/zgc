#include "KCardInst.h"
#include "../StaticTable/KAbilityStatic.h"
#include "../StaticTable/KGameStatic.h"
#include "KBattleGod.h"
#include "System/Misc/KStream.h"

void _removeFromCardList(KCardInstList& lst,KCardInst* card)
{
	KCardInstList::iterator it = _findCardIt(&lst,card);
	if(it!=lst.end()) lst.erase(it);
}

void _rndPickCard(KCardInstList& src,KCardInstList& des,int num,KCardStatic::CardDef cardDef)
{
	KCardInstList tmpLst;
	for(KCardInstList::iterator it=src.begin();it!=src.end();it++){
		if(cardDef==KCardStatic::card_null ||
			(*it)->GetType()==cardDef)
			tmpLst.push_back(*it);
	}

	for(int i=0;i<num;i++){
		int nRand = g_rnd.GetRandom(0,tmpLst.size());
		int pos=0;
		for(KCardInstList::iterator it=tmpLst.begin();it!=tmpLst.end();it++,pos++){
			if(nRand==pos){
				des.push_back(*it);
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
	return true;
}

void KCardInst::clone(KCardInst* card)
{
	m_attr.clone(&card->m_attr);
	m_pST = card->m_pST;
	m_attr.clearBit();
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

void KCardInst::init(int realId,KBattleGuy* owner,KCardStatic* st)
{
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
	m_attr.setSlot(KCardInst::enum_slot_fight);
}

void KCardInst::EnterFightField(int pos)
{
    m_attr.setPos(pos);
    m_attr.setSlot(KCardInst::enum_slot_fight);
	m_attr.setReady(0);
	KAttrStatic* pAttr = KGameStaticMgr::getSingleton().GetAttr(m_pST->GetID());
	if(pAttr){
		if(pAttr->IsRush()) m_attr.setReady(1);
		if(pAttr->IsGuide()) AddBuf(10001);
		if(pAttr->IsHide()) AddBuf(10002);
		if(pAttr->IsDist()) AddBuf(10003);
	}
}

void KCardInst::onTurnBegin(KBattleCtrlBase* ctrl)
{
	if(!FindBuf(KAbilityStatic::what_stun)){
		CardSlot slot = GetSlot();
		if(slot==KCardInst::enum_slot_fight ||
			slot==KCardInst::enum_slot_hero){
				m_attr.setReady(1);
		}
	} 
	m_attr.updateBufList();
	onCardAbility(ctrl,KAbilityStatic::when_turn_begin);
}

void KCardInst::OnTurnEnd()
{
	CardSlot slot = GetSlot();
	if(slot!=KCardInst::enum_slot_fight &&
		slot!=KCardInst::enum_slot_hero){
			return;
	}
}

void KCardInst::HpDouble()
{
	int hp = GetHp()*2;
	m_attr.setCurHp(hp);
	m_attr.setMaxHp(hp);
}

void KCardInst::ReturnHand()
{
	m_attr.clearBuf();
	m_attr.setReady(0);
	m_attr.setCurHp(m_attr.getMaxHp());
	m_attr.setSlot(KCardInst::enum_slot_hand);
}

void KCardInst::HpSet(int val)
{
	m_attr.setCurHp(val);
	m_attr.setMaxHp(val);
}

void KCardInst::AddHp(int val)
{
	m_attr.setCurHp(m_attr.getCurHp()+val);
	m_attr.setMaxHp(m_attr.getMaxHp()+val);
}

int KCardInst::Heal(int val)
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
	int ret = hp - m_attr.getCurHp();
	m_attr.setCurHp(hp);
	if(ret<0){
		OnAbility(KAbilityStatic::when_damaged);
	}
	return ret;
}

void KCardInst::AddAtk(int val)
{
	m_attr.setAddAtk(m_attr.getAddAtk()+val);
}

void KCardInst::OnAbility(KAbilityStatic::Enum_When when)
{
	KAbilityStatic* pAbility = FindBuf(when);
	if(!pAbility) return;
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_atk_add:
		{
			int atk = m_attr.getAddAtk() + pAbility->GetVal();
			m_attr.setAddAtk(atk);
		}
		break;
    default:
        break;
	}
}

int KCardInst::GetHp()
{
	return m_attr.getCurHp();
}

void KCardInst::AddBuf(int id)
{
	KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(id);
	if(pBuf) m_attr.AddBuf(pBuf);
}

void KCardInst::AddBuf(KAbilityStatic* pBuf)
{
	m_attr.AddBuf(pBuf);
}

void KCardInst::DelBuf(KAbilityStatic* pBuf)
{
	
	m_attr.DelBuf(pBuf);
}

void KCardInst::ClearBuf()
{
	KCardAbilityList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		KAbilityStatic* pBuf = *it;
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

KAbilityStatic* KCardInst::FindStaticAbility(KAbilityStatic::Enum_When when)
{
	KCardAbilityList abilityList;
	KGameStaticMgr::getSingleton().GetAbilityList(GetCardId(),abilityList,when);
	if(abilityList.empty()) return NULL;
	return *(abilityList.begin());
}

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
	if(pAbility){
		KBattleGod::getSingleton().DoCardAbility(ctrl,pAbility,this);
	}
}

void KCardInst::ReplaceST(int id)
{
	m_attr.setCardId(id);
	m_pST = KGameStaticMgr::getSingleton().GetCard(id);
}

const char* KCardInst::GetBasePosName(bool bMy)
{
	static char sz[64];
	switch(GetSlot()){
	case KCardInst::enum_slot_select:
		return "select_base";
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
	switch(GetType()){
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
	KAbilityStatic* pAbility = FindStaticAbility(when);
	if(!pAbility) return false;
	if(pAbility->IsArea()) return true;
	if(pAbility->GetWhich()==KAbilityStatic::which_i ||
		pAbility->GetWhich()==KAbilityStatic::which_u||
		pAbility->GetWhich()==KAbilityStatic::which_owner)
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
	so->WriteInt(m_attr.getRealID());
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
	if(GetType()==KCardStatic::card_skill|| GetType()==KCardStatic::card_hero)
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