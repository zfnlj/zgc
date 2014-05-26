#include "KBattleDeck.h"

#include "../StaticTable/KGameStatic.h"
#include "KCardInst.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "KBattleGuy.h"
#include "../Inc/KTypeDef.h"
#include "KBattleCtrlBase.h"
#include "System/Misc/KStream.h"
#include "../common/KCommonObj.h"


int tmpCard[MAX_GAME_PLAY_CARD]={10002,20051,20045,20049,20050,20005,
								 20004,20005,30001,20002,20002,20001,
								 20007,20003,30003,30001,20002,20001,
								 20001,20001,20002,30006,20002,20001,
								 20001,20001,20002,20002,20002,20001,20001};
static int g_inc = 1;
KBattleDeck::~KBattleDeck()
{
	Clear();
}

bool KBattleDeck::initGuy(KBattleGuy* guy)
{
	m_Owner = guy;
	return true;
}

bool KBattleDeck::AddCard(KCardInstList* list, KCardInst* card) 
{
	list->push_back(card);
	return true;
}

void KBattleDeck::RemoveCard(KCardInst* card)
{
	KCardInstList* oldList = QueryCardSet(card);
	if(oldList)_removeFromCardList(*oldList,card);
}

void KBattleDeck::updateCardSlot(KCardInst* card)
{
	KCardInstList* oldList = QueryCardSet(card);
	if(!oldList) card->SetOwner(m_Owner);
	KCardInstList* newList = QueryCardSet(card->GetSlot());
	if(oldList!=newList){
		if(oldList)_removeFromCardList(*oldList,card);
		if(newList) newList->push_back(card);
	}
}


void KBattleDeck::SetCurDeckDB(int* cardStore,int* cardDeck)
{
	for(int i=0;i<MAX_GAME_PLAY_CARD;i++)
	{
		m_CurDeckDB[i] = cardStore[cardDeck[i]];
	}
}

void KBattleDeck::initDeck(KDeckDefStatic* pDeckStatic)
{
	memset(m_CurDeckDB,0,sizeof(m_CurDeckDB));
	m_CurDeckDB[0] = pDeckStatic->getHero();
	KIntegerList tmpLst;
	pDeckStatic->GenCardList(tmpLst,pDeckStatic->IsRnd());
	int index = 1;
	for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();++it,index++)
	{
		m_CurDeckDB[index] = *it;
	}
}

bool KBattleDeck::createCards(void)
{

	KCardInst* pHero = KCardInst::create();
	if(m_CurDeckDB[0]==0) memcpy(m_CurDeckDB ,tmpCard,sizeof(tmpCard));

	KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(m_CurDeckDB[0]);
	pHero->init(g_inc++,m_Owner,pST);
	pHero->m_attr.setSlot(KCardInst::enum_slot_hero);
	AddCard(&m_HeroCardSet,pHero);

	for(int i=1;i<MAX_GAME_PLAY_CARD;i++){
		if(m_CurDeckDB[i]==0) break;
		CreateCard(m_CurDeckDB[i],KCardInst::enum_slot_slot);
	}
	return true;
}

KCardInst* KBattleDeck::CreateCard(int id,KCardInst::CardSlot slot)
{
	KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(id);
	CCAssert(pST , "testInit--Not find the card!");
	KCardInst* pCard = KCardInst::create();

	pCard->init(g_inc++,m_Owner,pST);
	pCard->m_attr.setSlot(slot);
	KCardInstList* list = QueryCardSet(slot);
	AddCard(list,pCard);
	return pCard;
}

KCardInst* KBattleDeck::SummonCard(int id)
{
	int pos = GetEmptyFightSlot();
	if(pos < 0) return NULL;
	KCardInst* pCard = CreateCard(id,KCardInst::enum_slot_fight);
	pCard->EnterFightField(pos);
	return pCard;
}

void KBattleDeck::onTurnBegin(KBattleCtrlBase* ctrl)
{
	KCardInstList tmpSet;
	_copyCardSet(&m_FightCardSet,&tmpSet,NULL,KCardInst::enum_slot_fight);
	for(KCardInstList::iterator it = tmpSet.begin();it!=tmpSet.end();it++){
		(*it)->onTurnBegin(ctrl);
	}
	GetHero()->onTurnBegin(ctrl);
}

void KBattleDeck::OnTurnEnd(KBattleCtrlBase* ctrl)
{
	KCardInstList tmpSet;
	_copyCardSet(&m_FightCardSet,&tmpSet,NULL,KCardInst::enum_slot_fight);
	for(KCardInstList::iterator it = tmpSet.begin();it!=tmpSet.end();it++){
		(*it)->OnTurnEnd(ctrl);
	}
	GetHero()->OnTurnEnd(ctrl);
}

void KBattleDeck::GetDefenderSet(KCardInstList* lst)
{
	for(KCardInstList::iterator it = m_FightCardSet.begin();it!=m_FightCardSet.end();it++){
		KCardInst* pCard = *it;
		if(!pCard->FindRealBuf(KAbilityStatic::what_hide)){
			lst->push_back(pCard);
		}
	}
}

void KBattleDeck::FindFightingGuider(KCardInstList* lst)
{
	for(KCardInstList::iterator it = m_FightCardSet.begin();it!=m_FightCardSet.end();it++){
		KCardInst* pCard = *it;
		if(pCard->FindRealBuf(KAbilityStatic::what_guide)){
			lst->push_back(pCard);
		}
	}
}



void KBattleDeck::Clear()
{
	memset(m_CurDeckDB,0,sizeof(m_CurDeckDB));
	_clearCardList(&m_HeroCardSet);
	_clearCardList(&m_HandCardSet);
	_clearCardList(&m_FightCardSet);
	_clearCardList(&m_EquipCardSet);
	_clearCardList(&m_SlotCardSet);
	_clearCardList(&m_TombCardSet);
    _clearCardList(&m_SecretCardSet);
}

void KBattleDeck::onCard2Tomb(KCardInst* card)
{
	card->onCard2Tomb();
	updateCardSlot(card);
	CCLog("onCard2Tomb=%s ,id=%d",card->GetST()->GetName(),card->GetRealId());
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_CARD2TOMB,card->GetRealId(),(unsigned long long)m_Owner->GetBattleCtrl()->GetWorld());
}

void KBattleDeck::Fight2Hand(KCardInst* card)
{
	card->ReturnHand();
	updateCardSlot(card);
	//KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_CARDMOVE,card->GetRealId(),(unsigned long long)m_Owner->GetBattleCtrl()->GetWorld());
}

void KBattleDeck::Hand2Secret(KCardInst* card,int pos)
{
	card->EnterSecretField(pos);
	updateCardSlot(card);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_USE_SECRET,card->GetRealId(),(unsigned long long)m_Owner->GetBattleCtrl()->GetWorld());
}

void KBattleDeck::Hand2Fight(KCardInst* pCard)
{
	int pos = GetEmptyFightSlot();
	Hand2Fight(pCard,pos);
}

void KBattleDeck::Hand2Fight(KCardInst* card,int pos)
{
	card->EnterFightField(pos);
	updateCardSlot(card);
}

void KBattleDeck::Hand2Tomb(KCardInst* card)
{
	card->m_attr.setSlot(KCardInst::enum_slot_tomb);
	updateCardSlot(card);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_CARDMOVE,card->GetRealId(),(unsigned long long)m_Owner->GetBattleCtrl()->GetWorld());
}

KCardInstList* KBattleDeck::QueryCardSet(KCardInst* card)
{
	if(_findCardIt(&m_HeroCardSet,card)!=m_HeroCardSet.end())
		return &m_HeroCardSet;
	if(_findCardIt(&m_HandCardSet,card)!=m_HandCardSet.end())
		return &m_HandCardSet;
	if(_findCardIt(&m_FightCardSet,card)!=m_FightCardSet.end())
		return &m_FightCardSet;
	if(_findCardIt(&m_EquipCardSet,card)!=m_EquipCardSet.end())
		return &m_EquipCardSet;
	if(_findCardIt(&m_SlotCardSet,card)!=m_SlotCardSet.end())
		return &m_SlotCardSet;
	if(_findCardIt(&m_TombCardSet,card)!=m_TombCardSet.end())
		return &m_TombCardSet;
    if(_findCardIt(&m_SecretCardSet,card)!=m_SecretCardSet.end())
		return &m_SecretCardSet;
	return NULL;
}

KCardInst* KBattleDeck::GetCard(int id)
{
	KCardInst* pCard = GetCard(&m_HandCardSet,id);
	if(pCard){
		return pCard;
	}
	pCard = GetCard(&m_FightCardSet,id);
	if(pCard){
		return pCard;
	}
	pCard = GetCard(&m_HeroCardSet,id);
	if(pCard){
		return pCard;
	}
	pCard = GetCard(&m_EquipCardSet,id);
	if(pCard){
		return pCard;
	}
	pCard = GetCard(&m_SlotCardSet,id);
	if(pCard){
		return pCard;
	}
	pCard = GetCard(&m_TombCardSet,id);
	if(pCard){
		return pCard;
	}
    pCard = GetCard(&m_SecretCardSet,id);
	if(pCard){
		return pCard;
	}
	return NULL;
}

KCardInst* KBattleDeck::GetCard(KCardInstList* lst,int id)
{
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		if( (*it)->GetRealId()==id ) return *it;
	}
	return NULL;
}

void KBattleDeck::CreateCloneCard(KCardInstList& lst,KCardInstList& newLst,KCardInst::CardSlot slot)
{
	for(KCardInstList::iterator it=lst.begin();it!=lst.end();++it){
		KCardInst* cloneCard = CreateCard((*it)->GetCardId(),slot);
		newLst.push_back(cloneCard);
	}
}

int KBattleDeck::DrawCard(int n,KCardInst::CardSlot slot,strCardAbilityResult* result)
{
	int ret = 0;
	KCardInstList tmpList;
	while(n>0){
		if (m_SlotCardSet.empty()){
			break;
		}
		KCardInst* card = (KCardInst*)m_SlotCardSet.front();
		m_SlotCardSet.pop_front();
		m_HandCardSet.push_back(card);
		card->m_attr.setSlot(slot);
		tmpList.push_back(card);
		if(result) result->SetDestVal(card->GetRealId(),0);
		n--;
		ret++;
	}
	if(!result) KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DRAWCARD,(unsigned long long)&tmpList,
													(unsigned long long)m_Owner->GetBattleCtrl()->GetWorld());
	return ret;
}

KCardInstList* KBattleDeck::QueryCardSet(int slot)
{
	switch((KCardInst::CardSlot)slot){
	case KCardInst::enum_slot_hero:
		return &m_HeroCardSet;
		break;
	case KCardInst::enum_slot_equip:
		return &m_EquipCardSet;
		break;
	case KCardInst::enum_slot_select:
	case KCardInst::enum_slot_hand:
		return &m_HandCardSet;
		break;
	case KCardInst::enum_slot_fight:
		return &m_FightCardSet;
		break;
	case KCardInst::enum_slot_slot:
		return &m_SlotCardSet;
		break;
	case KCardInst::enum_slot_tomb:
		return &m_TombCardSet;
		break;
    case KCardInst::enum_slot_secret:
            return &m_SecretCardSet;
            break;
     default:
            break;
	}
	return NULL;
}

void KBattleDeck::PickFighterNearby(KCardInstList* lst,KCardInst* me)
{
	if(me->GetSlot()!= KCardInst::enum_slot_fight) return;

	int mePos = me->m_attr.getPos();
	for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
			KCardInst* card = *it;
			if(card==me) continue;
			int cardPos = card->m_attr.getPos();
			int dist = (mePos>cardPos)? mePos-cardPos : cardPos-mePos;
			if(dist==1) lst->push_back(card);
	}
}

void KBattleDeck::PickCard(KCardInstList* lst,KCardInst::CardSlot slot,KCardInst* skip)
{
	switch(slot){
	case KCardInst::enum_slot_equip:
		_copyCardSet(&m_EquipCardSet,lst,skip);
		break;
	case KCardInst::enum_slot_hand:
		_copyCardSet(&m_HandCardSet,lst,skip);
		break;
	case KCardInst::enum_slot_fight:
		_copyCardSet(&m_FightCardSet,lst,skip);
		break;
	case KCardInst::enum_slot_slot:
		_copyCardSet(&m_SlotCardSet,lst,skip);
		break;
	case KCardInst::enum_slot_tomb:
		_copyCardSet(&m_TombCardSet,lst,skip);
		break;
    default:
        break;
	}
}

void KBattleDeck::onSelectHandCardOK(KCardInstList* lst)
{
	if(lst){
		for(KCardInstList::iterator it = lst->begin(); it!=lst->end();++it){
			KCardInst* card = *it;
			card->m_attr.setSlot(KCardInst::enum_slot_slot);
			m_SlotCardSet.push_back(card);
			_removeFromCardList(m_HandCardSet,card);
		}
		for(size_t i=0;i<lst->size();i++){
			KCardInst* card = m_SlotCardSet.front();
			m_SlotCardSet.pop_front();
			m_HandCardSet.push_back(card);
		}
	}
	for(KCardInstList::iterator it = m_HandCardSet.begin(); it!=m_HandCardSet.end();++it){
		(*it)->m_attr.setSlot(KCardInst::enum_slot_hand);
	}
}

void KBattleDeck::UpdateActor(float dt)
{
	UpdateActor(&m_EquipCardSet,dt);
	UpdateActor(&m_HandCardSet,dt);
	UpdateActor(&m_FightCardSet,dt);
	UpdateActor(&m_HeroCardSet,dt);
	UpdateActor(&m_TombCardSet,dt);
    UpdateActor(&m_SecretCardSet,dt);
}

void KBattleDeck::UpdateActor(KCardInstList* lst,float dt)
{
#ifdef _USE_COCOS2DX
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardInst* pCard = *it;
		if(pCard->getActor()) pCard->getActor()->update(dt);
	}
#endif
}

void KBattleDeck::QueryValidateHandCards(KCardInstList* lst,int curRes)
{
	for(KCardInstList::iterator it = m_HandCardSet.begin(); it!=m_HandCardSet.end();++it){
		KCardInst* pCard = *it;
		KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(pCard->GetCardId()*10);
		if(pAbility){
			if(pAbility->GetWhat()==KAbilityStatic::what_copy_fight||
				pAbility->GetWhat()==KAbilityStatic::what_control){
				int pos = GetEmptyFightSlot();
				if(pos<0) continue;;
			}
		}
		
		if(pCard->GetRealCost()<=curRes) lst->push_back(pCard);
	}
}

void KBattleDeck::QuerySleepFightCards(KCardInstList* lst)
{
	for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
		KCardInst* pCard = *it;
		if(!pCard->m_attr.getReady()) lst->push_back(pCard);
	}
}

void KBattleDeck::QueryValidateFightCards(KCardInstList* lst,int curRes)
{
	for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
		KCardInst* pCard = *it;
		if(pCard->m_attr.getReady()) lst->push_back(pCard);
	}
}

void KBattleDeck::QueryActiveDefendCards(KCardInstList* lst)
{
	KCardInst* pAtk = m_Owner->GetBattleCtrl()->GetCurSrcCard();
	if(!pAtk) return;

	KCardInstList lstGuider;
	KCardInstList lstDefender;
	if(pAtk->IsKindOf(KCardStatic::card_soldier)){
		for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
			KCardInst* pCard = *it;
			if(pCard->FindRealBuf(KAbilityStatic::what_guide)) lstGuider.push_back(pCard);
			if(pCard->IsActiveDefend()) lstDefender.push_back(pCard);
		}

		if(!lstGuider.empty()){
			_copyCardSet(&lstGuider,lst);
			return;
		}
		for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
			KCardInst* pCard = *it;
			if(pCard->FindRealBuf(KAbilityStatic::what_hide)) continue;
			lst->push_back(pCard);
		}

		KCardInst* pHero = GetHero();
		if(pHero->IsActiveDefend()) lst->push_back(pHero);
	}
}

size_t KBattleDeck::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	if(!serializeCardList(so,m_HeroCardSet))
		return 0;
	if(!serializeCardList(so,m_HandCardSet,true))
		return 0;
	if(!serializeCardList(so,m_FightCardSet))
		return 0;
	if(!serializeCardList(so,m_EquipCardSet))
		return 0;
	if(!serializeCardList(so,m_SlotCardSet,true))
		return 0;
	if(!serializeCardList(so,m_TombCardSet,true))
		return 0;
    if(!serializeCardList(so,m_SecretCardSet))
		return 0;
	return so->size() - pos;

}

size_t KBattleDeck::serializeCardList(KMemoryStream* so,KCardInstList& lst,bool newCard)
{
	size_t pos = so->size();
	BYTE n = lst.size();
	if(!so->WriteByte(n))
		return -1;
	for(KCardInstList::iterator it=lst.begin();it!=lst.end();++it){
		KCardInst* card = *it;
		if(newCard){
			if(!card->serializeDirty(so)) return 0;
		}else{
			if(!card->serialize(so)) return 0;
		}
		
	}
	return so->size() - pos;
}

BOOL KBattleDeck::deserialize(KMemoryStream* si)
{
	if(!deserializeCardList(si,m_HeroCardSet))
		return FALSE;
	if(!deserializeCardList(si,m_HandCardSet))
		return FALSE;
	if(!deserializeCardList(si,m_FightCardSet))
		return FALSE;
	if(!deserializeCardList(si,m_EquipCardSet))
		return FALSE;
	if(!deserializeCardList(si,m_SlotCardSet))
		return FALSE;
	if(!deserializeCardList(si,m_TombCardSet))
		return FALSE;
    if(!deserializeCardList(si,m_SecretCardSet))
		return FALSE;
	return TRUE;
}

BOOL KBattleDeck::deserializeCardList(KMemoryStream* si,KCardInstList& lst)
{
	_clearCardList(&lst);
	BYTE n;
	if(!si->ReadByte(n))
		return FALSE;
	for(int i=0;i<n;i++){
		KCardInst* card = KCardInst::create();
		if(!card->deserialize(si)) return FALSE;
		card->SetOwner(m_Owner);
		lst.push_back(card);
	}
	return TRUE;
}

size_t KBattleDeck::serializeDirty(KMemoryStream* so)
{
	size_t pos = so->size();
	KCardInstList tmpList;
	_fillDirtyCardList(m_HeroCardSet,tmpList);
	_fillDirtyCardList(m_HandCardSet,tmpList);
	_fillDirtyCardList(m_FightCardSet,tmpList);
	_fillDirtyCardList(m_EquipCardSet,tmpList);
	_fillDirtyCardList(m_TombCardSet,tmpList);
    _fillDirtyCardList(m_SecretCardSet,tmpList);
	unsigned char n = (unsigned char)tmpList.size();
	so->WriteByte(n);
	for(KCardInstList::iterator it=tmpList.begin();it!=tmpList.end();++it){
		KCardInst* card = *it;
		if(!card->serializeDirty(so)) return -1;
	}
	return so->size() - pos;
}

BOOL KBattleDeck::deserializeDirty(KMemoryStream* si)
{
	unsigned char n;
	if(!si->ReadByte(n))
		return FALSE;
	for(int i=0;i<n;i++){
		int realId;
		if(!si->ReadInt(realId)) return FALSE;
		KCardInst* card = GetCard(realId);
		//CCAssert(card , "deserialize error--Not find the card!");
		if(card){
			card->deserialize(si);
			updateCardSlot(card);
		}else{
			card = KCardInst::create();
			card->deserialize(si);
			KCardInstList* cardSet = QueryCardSet(card->GetSlot());
			if(cardSet) cardSet->push_back(card);
		}
	}
	return TRUE;
}

int KBattleDeck::GetEmptyFightSlotNum()
{
	int count = 0;
	bool posFlag[MAX_FIGHT_POS_NUM];
	memset(posFlag,0,sizeof(posFlag));
	for(KCardInstList::iterator it=m_FightCardSet.begin();it!=m_FightCardSet.end();++it){
		KCardInst* card = *it;
		posFlag[card->m_attr.getPos()]= true;
	}
	for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
		if(posFlag[i]==false) count++;
	}
	return count;
}

int KBattleDeck::GetEmptyFightSlot()
{
    bool posFlag[MAX_FIGHT_POS_NUM];
    memset(posFlag,0,sizeof(posFlag));
    for(KCardInstList::iterator it=m_FightCardSet.begin();it!=m_FightCardSet.end();++it){
		KCardInst* card = *it;
        posFlag[card->m_attr.getPos()]= true;
	}
    for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
        if(posFlag[i]==false) return i;
    }
    return -1;
    
}

bool KBattleDeck::IsEmptyFightSlot(int pos)
{
	for(KCardInstList::iterator it=m_FightCardSet.begin();it!=m_FightCardSet.end();++it){
		KCardInst* card = *it;
		if(card->m_attr.getPos()==pos) return false;
	}
	return true;
}
int KBattleDeck::GetRndEmptyFightSlot()
{
	bool posFlag[MAX_FIGHT_POS_NUM];
	memset(posFlag,0,sizeof(posFlag));
	for(KCardInstList::iterator it=m_FightCardSet.begin();it!=m_FightCardSet.end();++it){
		KCardInst* card = *it;
		posFlag[card->m_attr.getPos()]= true;
	}
	int nRand = g_rnd.GetRandom(0,MAX_FIGHT_POS_NUM);

	while(nRand >= 0){
		int oldRand = nRand;
		for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
			if(posFlag[i]==false){
				if(nRand==0)
					return i;
				else
					nRand--;
			}
		}
		if(oldRand==nRand)	break;
	}
	
	return -1;

}

void KBattleDeck::RndPickCard(KCardInstList& lst,int num,KCardInst::CardSlot slot,KCardStatic::CardDef cardDef)
{
	KCardInstList* src = QueryCardSet(slot);
	_rndPickCard(*src,lst,num,cardDef);
}

int KBattleDeck::GetEmptySecretSlot()
{
    bool posFlag[MAX_SECRET_POS_NUM];
    memset(posFlag,0,sizeof(posFlag));
    for(KCardInstList::iterator it=m_SecretCardSet.begin();it!=m_SecretCardSet.end();++it){
		KCardInst* card = *it;
        posFlag[card->m_attr.getPos()]= true;
	}
    for(int i=0;i<MAX_SECRET_POS_NUM;i++){
        if(posFlag[i]==false) return i;
    }
    return -1;
    
}

int KBattleDeck::GetSoldierNum()
{
	return m_FightCardSet.size();
}

int KBattleDeck::GetHurtedSoldierNum()
{
	int count=0;
	for(KCardInstList::iterator it=m_FightCardSet.begin();it!=m_FightCardSet.end();++it){
		KCardInst* card = *it;
		if(card->FindRealBuf(KAbilityStatic::what_hurted)) count++;
	}
	return count;
}

bool KBattleDeck::ExistCards()
{
	return ((m_FightCardSet.size() + m_HandCardSet.size() + m_SlotCardSet.size()) > 0);
}