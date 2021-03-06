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
#include "../PlayerCard/KPlayerCardDepot.h"
#include "assist/KSkillAssist.h"

#define  MAX_DECK_CARD_NUM 30
int tmpCard[MAX_GAME_PLAY_CARD]={10002,20008,30011,32003,31019,31020,
								 20002,20002,31021,31023,31024,31025,
								 20007,20003,31016,30001,20002,20001,
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
	m_heroSkillMgr.init(guy);
	
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

bool KBattleDeck::createDeck(KPlayerCardDepot* pDepot)
{
	if(!pDepot) return false;
	int heroId;
	KIntegerList tmpLst,rndLst;
	if(!pDepot->PickCurDeck(heroId,tmpLst)) return false;

	_RndIntegerList(tmpLst,rndLst);
	createDeck(heroId,rndLst);
	KCardInst* pHero = GetHero();
	KHeroDef heroDef;
	pDepot->PickCurHero(heroDef);
	pHero->AddHp(heroDef.GetStrong());
	return true;
}

void KBattleDeck::createDeck(KDeckDefStatic* pDeckStatic)
{
	KIntegerList tmpLst;
	pDeckStatic->GenCardList(tmpLst,pDeckStatic->IsRnd());
	createDeck(pDeckStatic->getHero(),tmpLst);
}

void KBattleDeck::createDeck(int heroId,KIntegerList& cardLst)
{
	Clear();

	KCardInst* pHero = KCardInst::create();
	pHero->init(g_inc++,m_Owner,heroId);
	pHero->m_attr.setSlot(KCardInst::enum_slot_hero);
	AddCard(&m_HeroCardSet,pHero);

	int num=0;
	for(KIntegerList::iterator it=cardLst.begin();it!=cardLst.end();++it,num++){
		if(num==MAX_DECK_CARD_NUM) break;
		CreateCard(*it,KCardInst::enum_slot_slot);
	}
}

bool KBattleDeck::createTestDeck(void)
{
	KIntegerList tmpLst;
	for(int i=1;i<MAX_GAME_PLAY_CARD;i++){
		tmpLst.push_back(tmpCard[i]);
	}
	createDeck(tmpCard[0],tmpLst);
	return true;
}

KCardInst* KBattleDeck::CreateCard(int id,KCardInst::CardSlot slot)
{
	KCardInst* pCard = KCardInst::create();

	pCard->init(g_inc++,m_Owner,id);
	pCard->m_attr.setSlot(slot);
	KCardInstList* list = QueryCardSet(slot);
	AddCard(list,pCard);
	return pCard;
}

KCardInst* KBattleDeck::SummonCard(int id)
{
	int pos = GetRndEmptyFightSlot();
	if(pos < 0) return NULL;
	KCardInst* pCard = CreateCard(id,KCardInst::enum_slot_fight);
	pCard->EnterFightField(pos);
	return pCard;
}

void KBattleDeck::TurnBeginDrawCard()
{
	int drawNum = 1;
	if(m_heroDef.GetFate()>g_rnd.GetRandom(0,400)){
		drawNum++;
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DOUBLEDRAW,0,
													(unsigned long long)m_Owner->GetBattleCtrl()->GetWorld());
	}
	DrawCard(drawNum);
}

void KBattleDeck::onTurnBegin(KBattleCtrlBase* ctrl,bool bFirstTurn)
{
	KCardInstList tmpSet;
	_copyCardSet(&m_FightCardSet,&tmpSet,NULL,KCardInst::enum_slot_fight);
	for(KCardInstList::iterator it = tmpSet.begin();it!=tmpSet.end();it++){
		(*it)->onTurnBegin(ctrl);
	}
	//GetHero()->onTurnBegin(ctrl);
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
		if(!pCard->FindBuf(KAbilityStatic::what_hide)){
			lst->push_back(pCard);
		}
	}
}

void KBattleDeck::FindFightingGuider(KCardInstList* lst)
{
	for(KCardInstList::iterator it = m_FightCardSet.begin();it!=m_FightCardSet.end();it++){
		KCardInst* pCard = *it;
		if(pCard->FindBuf(KAbilityStatic::what_guide)){
			lst->push_back(pCard);
		}
	}
}



void KBattleDeck::Clear()
{
	m_heroDef.Clear();
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

//void KBattleDeck::Hand2Fight(KCardInst* pCard,int pos)
//{
//	int pos2  = pCard->m_attr.getPos();
//	int pos = GetEmptyFightSlot();
//	CCAssert(pos>=0 , "Failed to find empty fight slot");
//	Hand2Fight(pCard,pos);
//}

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

bool KBattleDeck::IsEmptyFightPos(int pos)
{
	if(pos<0) return false;
	if(pos>=MAX_FIGHT_POS_NUM) return false;
	for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
		KCardInst* card = *it;
		if(card->m_attr.getPos()==pos) return false;
	}
	return true;
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
			
			KCardInst* newcard = m_SlotCardSet.front();
			m_SlotCardSet.pop_front();

			_replaceFromCardList(m_HandCardSet,card,newcard);
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
	bool NoFighterSlot = (GetEmptyFightSlotNum()==0);
	for(KCardInstList::iterator it = m_HandCardSet.begin(); it!=m_HandCardSet.end();++it){
		KCardInst* pCard = *it;
		if(pCard->IsKindOf(KCardStatic::card_soldier)&& NoFighterSlot) continue;
		KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId(pCard->GetCardId()*10);
		if(pAbility){
			if(pAbility->GetWhat()==KAbilityStatic::what_copy_fight||
				pAbility->GetWhat()==KAbilityStatic::what_control){
				int pos = GetEmptyFightSlot();
				if(pos<0) continue;;
			}
			if(pAbility->GetWhat()==KAbilityStatic::what_copy_hand){
				KBattleDeck& deck = m_Owner->GetBattleCtrl()->GetDefGuy()->GetDeck();
				KCardInstList* cardSet = deck.QueryCardSet(KCardInst::enum_slot_hand);
				if(cardSet->empty()) continue;
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
			if(pCard->FindBuf(KAbilityStatic::what_guide)) lstGuider.push_back(pCard);
			if(pCard->IsActiveDefend()) lstDefender.push_back(pCard);
		}

		if(!lstGuider.empty()){
			_copyCardSet(&lstGuider,lst);
			return;
		}
		for(KCardInstList::iterator it = m_FightCardSet.begin(); it!=m_FightCardSet.end();++it){
			KCardInst* pCard = *it;
			if(pCard->FindBuf(KAbilityStatic::what_hide)) continue;
			lst->push_back(pCard);
		}

		KCardInst* pHero = GetHero();
		if(pHero->IsActiveDefend()) lst->push_back(pHero);
	}
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
		if(card->m_attr.getPos()<0) continue;
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
		if(card->m_attr.getPos()<0) continue;
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
		if(card->m_attr.getPos()<0) continue;
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
		if(card->FindBuf(KAbilityStatic::what_hurted)) count++;
	}
	return count;
}

bool KBattleDeck::ExistCards()
{
	return ((m_FightCardSet.size() + m_HandCardSet.size() + m_SlotCardSet.size()) > 0);
}

void KBattleDeck::initDeck(KDeckDefStatic* pDeckDef,bool bSelectCard)
{
	if(!pDeckDef) return;
	createDeck(pDeckDef);
	m_heroDef.init(pDeckDef);
	m_heroDef.CalcLev();
	SetHeroSkill();
	GetHero()->HpSet(pDeckDef->getHeroHp()+pDeckDef->getHeroStrong());
	DrawCard(pDeckDef->getDrawNum(),(bSelectCard)?KCardInst::enum_slot_select:KCardInst::enum_slot_hand);
}

void KBattleDeck::SetHeroSkill()
{
	m_heroSkillMgr.SetHero(&m_heroDef);
}