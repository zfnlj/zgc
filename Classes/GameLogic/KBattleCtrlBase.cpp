#include "KBattleCtrlBase.h"
#include "KBattleGuy.h"
#include "KBattleAI.h"
#include "KBattleGod.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "KGameDef.h"
#include "assist/KLogAssist.h"
#include "assist/KSkillAssist.h"
#include "../Inc/KTypeDef.h"
#include "System/Misc/KStream.h"
#include "../Quest/KQuestNew.h"

bool KBattleCtrlBase::init(void* w)
{
	m_CurPlayGuy = m_pMainPlayer = NULL;
	m_state = battle_null;
	m_WaitTime = 0;
	m_world = w;
	m_bDirty = false;
	m_bNetReady = false;
	m_pBattleQuest = NULL;
	m_waitDramaElapsed = 0;
	return true;
}

void KBattleCtrlBase::UpdateBattleGuy(float dt)
{
	if(m_state==battle_game_end) return;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		(*it)->update(dt);
	}
}

void KBattleCtrlBase::update(float dt)
{
	if(m_waitDramaElapsed>dt){
		m_waitDramaElapsed -= dt;
	}else if(m_waitDramaElapsed>0.0f){
		m_waitDramaElapsed = 0.0f;
	}

	UpdateBattleGuy(dt);
	if(IsServerSide()){
		UpdateDirtyInfo();
		switch(m_state){
			case battle_init:
				BattleInit();
				break;
			case quest_battle_init:
				QuestBattleInit(m_pBattleQuest);
				break;
			case battle_select_turnplayer:
				SelectTurnPlayer();
				break;
			case battle_select_handcard:
				SelectHandCard(dt);
				break;
			case battle_turn_begin:
				TurnBegin();
				break;
			case battle_play:
				PlayCard(dt);
  				break;
			case battle_turn_end:
				TurnEnd();
				break;
			case battle_game_end:
				GameEnd();
				break;
			default:
				break;
		}
	}
}

KCardInst* KBattleCtrlBase::GetCard(int id)
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		KCardInst* pCard = (*it)->GetDeck().GetCard(id);
		if(pCard){
			return pCard;
		}
	}
	return NULL;
}

void KBattleCtrlBase::onCard2Tomb(KCardInst* card)
{
	if(card->GetSlot()==KCardInst::enum_slot_hero){
		card->m_attr.setSlot(KCardInst::enum_slot_tomb);
		CCLog("Hero is dead! %s ,id=%d",card->GetST()->GetName(),card->GetRealId());
		StateJump(battle_game_end);
		return;
	}
	FBattleGuy* guy = GetCardOwner(card);
	if(guy){
		guy->GetImp()->GetDeck().onCard2Tomb(card);
	}
	onSoldier2TombMsg(card);
}



void KBattleCtrlBase::StartGame(BattleState st,KBattleGuy* p1,KBattleGuy* p2,KBattleGuy* p3,KBattleGuy* p4)
{
	m_state = st;//;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		(*it)->free();
	}
	m_BattleGuyList.clear();
	m_BattleGuyList.push_back(p1);
	m_BattleGuyList.push_back(p2);
	p1->SetBattleCtrl(this);
	p2->SetBattleCtrl(this);
	if(p3){
		m_BattleGuyList.push_back(p3);
		p3->SetBattleCtrl(this);
	}
	if(p4){
		m_BattleGuyList.push_back(p4);
		p4->SetBattleCtrl(this);
	}
}


void KBattleCtrlBase::DoSelectHandCard(UINT64 guyId,int n)
{
	if(n>3)
		return;
	KBattleGuy* guy = GetGuy(guyId);
	if(!guy || !guy->IsSelectHandCard())
		return;

	guy->GetDeck().DrawCard(n,KCardInst::enum_slot_select);
}

KBattleGuy* KBattleCtrlBase::GetGuy(UINT64 id)
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if((*it)->GetGuyId()==id) return *it;
	}
	return NULL;
}

void KBattleCtrlBase::BattleInit()
{
	
	m_CurPlayGuy = m_BattleGuyList.front();
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		(*it)->onBattleInit(*it==m_CurPlayGuy,0);
	}
	m_bFirstTurn = true;
	m_WaitTime = 0;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_INIT,(unsigned long long)&m_BattleGuyList,(unsigned long long)m_world);
	StateJump(battle_select_handcard);
	m_bDirty = true;
}

void KBattleCtrlBase::DoSelectBeginCard(KBattleGuy* guy,KCardInstList* arr)
{
	if(m_state!=battle_select_handcard)
		return;
	guy->DoSelectBeginCard(arr);
}
void KBattleCtrlBase::SelectHandCard(float dt)
{
	bool selectOK = true;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		(*it)->SelectHandCard();
		if((*it)->IsSelectHandCard()){
			selectOK = false;
		}
	}

	m_WaitTime += dt;
	if(m_WaitTime>30 || selectOK){
		for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
			KBattleGuy* guy = (*it);
			if(guy->IsSelectHandCard()){
				guy->DoSelectBeginCard(NULL);
			}else{
				guy->GetDeck().onSelectHandCardOK(NULL);
			}
		}
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_HANDCARD_READY,0,(unsigned long long)m_world);
		StateJump(battle_turn_begin);
	}
}

void KBattleCtrlBase::SelectTurnPlayer()
{
	KBattleGuyList::iterator it = m_BattleGuyList.begin();
	for(;it!=m_BattleGuyList.end();it++){
		if(*it != m_CurPlayGuy) continue;
		it++;
		break;
	}
	if(it == m_BattleGuyList.end()){
		m_CurPlayGuy = m_BattleGuyList.front();
	}else{
		m_CurPlayGuy = *it;
	}

	StateJump(battle_turn_begin);
	m_bDirty = true;
}

void KBattleCtrlBase::TurnBegin()
{
	KLogAssist::_turnBeginLog(m_CurPlayGuy);

	KBattleGod::getSingleton().OnTurnBegin();
	m_CurPlayGuy->onTurnBegin(this,m_bFirstTurn);
    
	m_bFirstTurn = false;
	if(IsServerSide()) KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_TURNBEGIN,(unsigned long long)m_CurPlayGuy,(unsigned long long)m_world);
	m_CurOp.Empty();
	StateJump(battle_play);
}

KBattleGuy* KBattleCtrlBase::GetDefGuy()
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if(*it == m_CurPlayGuy) continue;
		return *it;
	}
	return NULL;
}

void KBattleCtrlBase::PlayCard(float dt)
{
	bool ret =KBattleGod::getSingleton().OnPlayCard(this,&m_CurOp);
	m_CurPlayGuy->onPlayCard(dt,ret);
	if(m_CurPlayGuy->IsPlayTimeOut()&&!IsGameEnd()){
		StateJump(battle_turn_end);
	}
}

bool KBattleCtrlBase::IsGameEnd()
{
	return m_state==battle_game_end;
}

void KBattleCtrlBase::TurnEnd()
{
	CCLog("TurnEnd");
	m_CurPlayGuy->GetDeck().OnTurnEnd();
	KBattleGod::getSingleton().OnTurnEnd();
	StateJump(battle_select_turnplayer);
}

void KBattleCtrlBase::GameEnd()
{
	KBattleGuy* winner = NULL;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		KBattleGuy* guy = *it;
		KCardInst* pHero = guy->GetDeck().GetHero();
		if(pHero->GetSlot()!=KCardInst::enum_slot_tomb) winner = guy;
		guy->onGameEnd();
	}
#ifdef _USE_COCOS2DX
	if(m_pBattleQuest && winner==m_pMainPlayer){
		KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(m_pBattleQuest->m_battleField);
		KDynamicWorld::getSingleton().onKillMonster(pBattleStatic->GetMonster());
	}
#endif
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_GAMEEND,(unsigned long long)winner,(unsigned long long)m_world);

	StateJump(battle_null);
}


void KBattleCtrlBase::DoPlayerOp(int src,int des)
{
	if(m_state!=battle_play){
		return;
	}
	CCAssert(!m_CurOp.IsOK() , "Cur Op isn't Empty!");
	m_CurOp._src = src;
	m_CurOp._des = des;
}

int KBattleCtrlBase::GetCurSelSrc()
{
	return m_CurOp._src;
}

KCardInst* KBattleCtrlBase::GetCurSrcCard()
{
	return GetCard(m_CurOp._src);
}

void KBattleCtrlBase::DoSelectCard(KCardInst* card)
{
	switch(card->GetSlot()){
	case KCardInst::enum_slot_hand:
		m_CurOp._src = card->GetRealId();
		break;
	case KCardInst::enum_slot_fight:
	case KCardInst::enum_slot_hero:
		if(m_CurOp._src>0){
			m_CurOp._des = card->GetRealId();
			OpDone();
		}else{
			m_CurOp._src = card->GetRealId();
		}
		break;
    default:
        break;
	}
}

void KBattleCtrlBase::OpSetSlot(int slot)
{
	m_CurOp._slot = slot;
	KCardInst* card = GetCurSrcCard();
	if(!card) return;
	KAbilityStatic* pAbility = card->FindBuf(KAbilityStatic::when_enter);
	KCardInstList arrGreen,arrRed;
	QueryEnterFightTarget(card,&arrGreen,&arrRed);
	int n = arrGreen.size()+arrRed.size();
	
	if(!pAbility ||    //check if need to select target....
		pAbility->IsArea() ||
		n<=1)
	{
		m_CurOp._ok = true;
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_OPDONE,(unsigned long long)&m_CurOp,0);
	}
}

void KBattleCtrlBase::OpDone()
{
	m_CurOp._ok = true;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_OPDONE,(unsigned long long)&m_CurOp,0);
}

void KBattleCtrlBase::DoEndTurn()
{
	m_CurPlayGuy->SetPlayTimeOut();
}

bool KBattleCtrlBase::QueryEnterFightTarget(KCardInst*  card,KCardInstList* arrGreen,KCardInstList* arrRed)
{
	if(card->GetType()!=KCardStatic::card_soldier) return false;
	if(card->GetSlot()!=KCardInst::enum_slot_hand) return false;
	if(m_CurOp._slot<0) return false;
	KAbilityStatic* pAbility = card->FindBuf(KAbilityStatic::when_enter);
	if(!pAbility) return false;
	KCardInstList* lst = (pAbility->IsGood())? arrGreen: arrRed;
	KSkillAssist::_fillAbilityTarget(this,card,pAbility,lst);
	return true;
}

void KBattleCtrlBase::QuerySkillTarget(KCardInst* skill,KCardInstList* arrGreen,KCardInstList* arrRed)
{
	if(skill->GetType()!=KCardStatic::card_skill &&
		skill->GetType()!=KCardStatic::card_hero)
		return;
	KCardAbilityList abilityList;
	KGameStaticMgr::getSingleton().GetAbilityList(skill->GetCardId(),abilityList);
	for(KCardAbilityList::iterator it=abilityList.begin();it!=abilityList.end();++it){
		KAbilityStatic* pAbility = *it;
		KCardInstList* lst = (pAbility->IsGood())? arrGreen: arrRed;
		KSkillAssist::_fillAbilityTarget(this,skill,pAbility,lst);
	}
}

void KBattleCtrlBase::PlayWithAI()
{
	CCLog("KDynamicWorld::PlayWithAI");
	m_pMainPlayer = KBattleGuy::create();
	KBattleAI* pAI = KBattleAI::create();
	m_bSelectCard = true;
	StartGame(battle_init,m_pMainPlayer,pAI);
}

void KBattleCtrlBase::PlayQuestBattle(KQuestNew* pQuest)
{
	KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(pQuest->m_battleField);
	m_pBattleQuest = pQuest;
	m_pMainPlayer = KBattleGuy::create();
	KBattleAI* pAI = KBattleAI::create();
	m_bSelectCard =pBattleStatic->IsSelectCard();
	StartGame(quest_battle_init,m_pMainPlayer,pAI);
}

FBattleGuy* KBattleCtrlBase::GetOtherPlayer()
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if(*it!=m_pMainPlayer) return (*it)->GetFacade();
	}
	return NULL;
}

KCardInstList* KBattleCtrlBase::GetCardSet(KCardInst* card)
{
	KCardInstList* lst = m_pMainPlayer->GetDeck().QueryCardSet(card->GetSlot());
	if(_findCardIt(lst,card)!=lst->end()) return lst;

	lst = GetOtherPlayer()->QueryCardSet(card->GetSlot());
	if(_findCardIt(lst,card)!=lst->end()) return lst;
	CCAssert(false , "Error to  GetCardSet!");
	return NULL;
}

void KBattleCtrlBase::DoPlayerOpOK(int src,int des,int slot)
{
	DoPlayerOp(src,des);
	OpSetSlot(slot);
	OpDone();
}

KBattleDeck* KBattleCtrlBase::GetCardDeck(KCardInst* card)
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if((*it)->GetDeck().GetCard(card->GetRealId())!=NULL) return &((*it)->GetDeck());
	}
	return NULL;
}

FBattleGuy* KBattleCtrlBase::GetCardOwner(KCardInst* card)
{

	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if((*it)->GetDeck().GetCard(card->GetRealId())!=NULL) return (*it)->GetFacade();
	}
	return NULL;
}


FBattleGuy* KBattleCtrlBase::GetCardOwner(int id)
{
    
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if((*it)->GetDeck().GetCard(id)!=NULL) return (*it)->GetFacade();
	}
	return NULL;
}

bool KBattleCtrlBase::IsMyTurn()
{
	return m_pMainPlayer==m_CurPlayGuy;
}


bool KBattleCtrlBase::IsMyCard(KCardInst* obj)
{
	if(!m_pMainPlayer) return false;
	KCardInst* pCard = (KCardInst*)obj;
	return (m_pMainPlayer->GetDeck().GetCard(pCard->GetRealId())!=NULL);
}

bool KBattleCtrlBase::IsShowBack(KCardInst* card)
{
	if(IsMyCard(card)){
		return false;
	}else{
		if(card->GetSlot()==KCardInst::enum_slot_hand||
			card->GetSlot()==KCardInst::enum_slot_select)
			return true;
		else
			return false;
	}
}

FBattleGuy* KBattleCtrlBase::GetMainPlayer()
{ 
	if(m_pMainPlayer){
		return m_pMainPlayer->GetFacade();
	}else{
		return NULL;
	}
}

void KBattleCtrlBase::deserializeDirty(UINT64 guyId,KMemoryStream* si)
{
	KBattleGuy* guy = GetGuy(guyId);
	if(!guy) return;
	guy->deserializeDirty(si);
}

void KBattleCtrlBase::deserializeBattleGuy(UINT64 guyId,KMemoryStream* si,bool bMainPlayer)
{
	KBattleGuy* guy = GetGuy(guyId);
	if(!guy){
		guy = KBattleGuy::create();
		m_BattleGuyList.push_back(guy);
	}
	guy->deserialize(si);
	guy->SetBattleCtrl(this);
	if(bMainPlayer) m_pMainPlayer = guy;
}

size_t KBattleCtrlBase::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteInt(m_state))
		return 0;
	UINT64 playId = (m_CurPlayGuy)? m_CurPlayGuy->GetGuyId():0;
	if(!so->WriteUint64(playId))
		return 0;
	return so->size() - pos;

}

bool KBattleCtrlBase::deserialize(KMemoryStream* si)
{
	int statVal = 0;
	if(!si->ReadInt(statVal))
		return false;
	UINT64 playId;
	if(!si->ReadUint64(playId))
		return false;
	KBattleGuy* guy = GetGuy(playId);
	m_CurPlayGuy = guy;
	m_state = (BattleState)statVal;
	return true;
}

void KBattleCtrlBase::UpdateDirtyInfo()
{
	if(!m_world) return;
	if(!m_bDirty) return;
	m_bDirty = false;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DIRTY,0,(unsigned long long)m_world);
}

void KBattleCtrlBase::onNetClose()
{
	init(NULL);
	m_bNetReady = false;
}

bool KBattleCtrlBase::IsServerSide()
{ 
	return (m_world!=NULL||!m_bNetReady);
}

void KBattleCtrlBase::onSoldier2TombMsg(KCardInst* card)
{
	if(card->GetType()!=KCardStatic::card_soldier) return;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		KBattleGuy* guy = *it;
		KCardInstList* fightSlot = guy->GetDeck().QueryCardSet(KCardInst::enum_slot_fight);
		for(KCardInstList::iterator itor = fightSlot->begin();itor!=fightSlot->end();++itor){
			(*itor)->onCardAbility(this,KAbilityStatic::when_soldier_dead);
		}
	}
}

void KBattleCtrlBase::QuestBattleInit(KQuestNew* pQuest)
{

	KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(pQuest->m_battleField);
	if(!pBattleStatic) return;
	if(pBattleStatic->IsMyFirst()){
		m_CurPlayGuy = m_pMainPlayer;
	}else{
		for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
			if(*it!=m_pMainPlayer){
				m_CurPlayGuy = *it;
				break;
			}
		}
	}

	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if(*it==m_pMainPlayer){
			(*it)->onBattleInit(m_CurPlayGuy==*it,pBattleStatic->GetMyDeck(),pBattleStatic->IsSelectCard());
		}else{
			(*it)->onBattleInit(m_CurPlayGuy==*it,pBattleStatic->GetYourDeck(),pBattleStatic->IsSelectCard());
		}
	}

	m_bFirstTurn = true;
	m_WaitTime = 0;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_INIT,(unsigned long long)&m_BattleGuyList,(unsigned long long)m_world);
	if(pBattleStatic->IsSelectCard()){
		StateJump(battle_select_handcard);
	}else{
		StateJump(battle_turn_begin);
	}
	m_bDirty = true;
}

void KBattleCtrlBase::onCardSwitchOwner(KCardInst* pSrc,KCardInst* pDes)
{
	KBattleGuy* newOwner = pSrc->GetOwner();
	KBattleGuy* oldOwner = pDes->GetOwner();

	int slot = newOwner->GetDeck().GetRndEmptyFightSlot();
	pDes->EnterFightField(slot);
	oldOwner->GetDeck().RemoveCard(pDes);
	newOwner->GetDeck().updateCardSlot(pDes);
	newOwner->onCardEnterCtrl(pDes);
	oldOwner->onCardLeaveCtrl(pDes);
}

FBattleGuy* KBattleCtrlBase::GetCurPlayer()
{
	if(!m_CurPlayGuy) return NULL;
	return m_CurPlayGuy->GetFacade();
}