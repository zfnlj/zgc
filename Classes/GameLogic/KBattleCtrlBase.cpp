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
#include "assist/KBattleCtrlAssist.h"
#include "../PlayerCard/KTowerAssist.h"
#ifdef _USE_COCOS2DX
#include "../GameRecord/KGameRecordMgr.h"
#endif

bool KBattleCtrlBase::g_bDebug = false;
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

void KBattleCtrlBase::Clear()
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		KBattleGuy::Free(*it);
	}
	m_BattleGuyList.clear();
	m_pMainPlayer = NULL;
	m_CurPlayGuy = NULL;
	m_CurOp.Empty();
	m_state = battle_null;
}

void KBattleCtrlBase::UpdateBattleGuy(float dt)
{
	//if(m_state==battle_game_end) return;
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
			case battle_turn_begin_hero:
				TurnBeginHero();
				break;
			case battle_turn_end_hero:
				TurnEndHero();
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
			case battle_turn_end_ok:
				TurnEndOk();
				break;
			case battle_game_end:
				GameEnd(dt);
				break;
			case battle_wait_drama:
				WaitDrama();
				break;
			default:
				break;
		}
	}
}



void KBattleCtrlBase::onCard2Tomb(KCardInst* card)
{
	
	if(card->IsKindOf(KCardStatic::card_hero)){
		card->m_attr.setSlot(KCardInst::enum_slot_tomb);
		CCLog("Hero is dead! %s ,id=%d",card->GetST()->GetName(),card->GetRealId());
		if(!card->IsClone())StateJump(battle_game_end);
		return;
	}
	FBattleGuy* guy = GetCardOwner(card);
	if(guy){
		guy->GetImp()->GetDeck().onCard2Tomb(card);
	}
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
	KBattleGuy* guy = KBattleCtrlAssist::GetGuy(this,guyId);
	if(!guy || !guy->IsSelectHandCard())
		return;

	guy->GetDeck().DrawCard(n,KCardInst::enum_slot_select);
}



void KBattleCtrlBase::BattleInit()
{
	RndSelectFirstPlayer();
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
		StateDelayJump(battle_turn_begin,2.0f);
	}
}

void KBattleCtrlBase::StateDelayJump(BattleState newState,float delay)
{
	m_waitDramaElapsed = delay;
	JumpOnDrama(newState);
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

	if(!m_bFirstTurn){
		m_CurPlayGuy->GetDeck().TurnBeginDrawCard();
		AddDramaElapsed(2.0f);
	}
	m_bFirstTurn = false;
	m_CurPlayGuy->onTurnBegin(this,m_bFirstTurn);
	
	if(IsServerSide()) KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_TURNBEGIN,(unsigned long long)m_CurPlayGuy,(unsigned long long)m_world);
	
	m_CurOp.Empty();
	
	DoCardEvtList(NULL);
	JumpOnDrama(battle_turn_begin_hero);
}

void KBattleCtrlBase::TurnBeginHero()
{
	m_CurPlayGuy->GetDeck().m_heroSkillMgr.onTurnBegin(this);
	JumpOnDrama(battle_play);
}

void KBattleCtrlBase::TurnEndHero()
{
	m_CurPlayGuy->GetDeck().m_heroSkillMgr.onTurnEnd(this);
	JumpOnDrama(battle_turn_end_ok);
}
void KBattleCtrlBase::WaitDrama()
{
	if(IsWaitDrama()){
		return;
	}
	StateJump(m_nextState);
}

KBattleGuy* KBattleCtrlBase::GetDefGuy()
{
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if(*it == m_CurPlayGuy) continue;
		return *it;
	}
	return NULL;
}

int KBattleCtrlBase::GetFighterNum()
{
	int ret = 0;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		ret += (*it)->GetFighterNum();
	}
	return ret;
}

void KBattleCtrlBase::PlayCard(float dt)
{
	bool ret =KBattleGod::getSingleton().OnPlayCard(this,&m_CurOp);
	m_CurPlayGuy->onPlayCard(dt,ret);
	if(m_CurPlayGuy->IsPlayTimeOut()&&!IsGameEnd()){
#ifdef _USE_COCOS2DX
		KGameRecordMgr::getSingleton().onMouseEvt(KRecordUIMouseData::evt_turn_end);
#endif
		StateJump(battle_turn_end);
	}
	if(!m_pMainPlayer->GetDeck().ExistCards()&& m_pBattleQuest && !IsGameEnd() ){
		m_pMainPlayer->GetDeck().GetHero()->m_attr.setSlot(KCardInst::enum_slot_tomb);
		StateJump(battle_game_end);
	}
}

bool KBattleCtrlBase::IsGameEnd()
{
	return m_state==battle_game_end;
}

void KBattleCtrlBase::TurnEndOk()
{
	StateJump(battle_select_turnplayer);
	if(IsServerSide()) KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_TURNEND,(unsigned long long)m_CurPlayGuy,(unsigned long long)m_world);
}

void KBattleCtrlBase::TurnEnd()
{
	CCLog("TurnEnd");
	m_CurPlayGuy->onTurnEnd(this);
	if(IsGameEnd()){
		return;
	}else{
		StateJump(battle_turn_end_hero);
		DoCardEvtList(NULL);
	}
}

void KBattleCtrlBase::GameEnd(float dt)
{
	strGameResult result;
	
	memset(&result,0,sizeof(result));
	if(IsWaitDrama()){
		for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
			KBattleGuy* guy = *it;
			guy->GetDeck().UpdateActor(dt);
		}
		return;
	}

	KBattleGuy* winner = NULL;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		KBattleGuy* guy = *it;
		KCardInst* pHero = guy->GetDeck().GetHero();
		if(pHero->GetSlot()!=KCardInst::enum_slot_tomb) winner = guy;
		guy->onGameEnd();
	}
#ifdef _USE_COCOS2DX
	if(m_pBattleQuest){
		if( winner==m_pMainPlayer){
			KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(m_pBattleQuest->GetBattleField());
			KDynamicWorld::getSingleton().onBattleWin(m_pBattleQuest->GetID(),pBattleStatic->GetMonster(),m_sceneType);
		}else{
			KDynamicWorld::getSingleton().onBattleFailed(m_pBattleQuest->GetID(),m_sceneType);
		}
	}
#endif
	StateJump(battle_null);

	result._winner = winner->GetFacade();
	result._questId = (m_pBattleQuest)? m_pBattleQuest->GetID():0;
	result._type = m_sceneType;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_GAMEEND,(unsigned long long)&result,(unsigned long long)m_world);

	
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
	return KBattleCtrlAssist::GetCard(this,m_CurOp._src);
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
	KCardInst* card = GetCurSrcCard();
	if(!card) return;
	KAbilityStatic* pAbility = NULL;
	int n=0;
	if(m_CurOp._slot<0){
		m_CurOp._slot = slot;
		pAbility = KSkillAssist::_findStaticAbility(card->GetCardId(),KAbilityStatic::when_enter);
		KCardInstList arrGreen,arrRed;
		KBattleCtrlAssist::QueryEnterFightTarget(this,card,&arrGreen,&arrRed);
		n = arrGreen.size()+arrRed.size();
	}
	
	
	
	if(!pAbility ||    //check if need to select target....
		pAbility->IsTargetSure()||
		pAbility->IsArea()||
		n==0)
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

void KBattleCtrlBase::PlayWithAI()
{
	CCLog("KDynamicWorld::PlayWithAI");
	m_pMainPlayer = KBattleGuy::create();
	KBattleAI* pAI = KBattleAI::create();
	m_bSelectCard = true;
	StartGame(battle_init,m_pMainPlayer,pAI);
}

void KBattleCtrlBase::PlayAutoQuestBattle(KQuestNew* pQuest)
{
	KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(pQuest->GetBattleField());
	m_pBattleQuest = pQuest;
	m_pMainPlayer = KBattleAI::create();
	KBattleAI* pAI = KBattleAI::create();
	m_bSelectCard =pBattleStatic->IsSelectCard();
	StartGame(quest_battle_init,m_pMainPlayer,pAI);
}

void KBattleCtrlBase::Play(KQuestNew* pQuest,Scene_type tp)
{
	m_sceneType = tp;
	KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(pQuest->GetBattleField());
	m_pBattleQuest = pQuest;
#ifdef _USE_COCOS2DX
	if(KGameRecordMgr::getSingleton().StartPlay(pBattleStatic->GetRec())) return;
#endif
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
	return card->GetOwner()->GetFacade();
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
	if(!m_pMainPlayer) return false;
	return m_pMainPlayer==m_CurPlayGuy;
}


bool KBattleCtrlBase::IsMyCard(KCardInst* obj)
{
	if(!m_pMainPlayer) return false;
	return (m_pMainPlayer==obj->GetOwner());
}

bool KBattleCtrlBase::IsShowBack(KCardInst* card)
{
	return false;
	//if(g_bDebug) return false;
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

void KBattleCtrlBase::RndSelectFirstPlayer()
{
	int nRand = g_rnd.GetRandom(0,m_BattleGuyList.size());
	int pos=0;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++,pos++){
		if(pos==nRand){
			m_CurPlayGuy = *it;
			break;
		}
	}
}

void KBattleCtrlBase::QuestBattleInit(KQuestNew* pQuest)
{
	KBattleFieldStatic* pBattleStatic = KGameStaticMgr::getSingleton().GetBattleField(pQuest->GetBattleField());
	if(!pBattleStatic) return;
	KBattleFieldStatic::enum_select_first select_enum = (KBattleFieldStatic::enum_select_first)pBattleStatic->GetFirstType();
	switch(select_enum){
	case KBattleFieldStatic::select_my:
		m_CurPlayGuy = m_pMainPlayer;
		break;
	case KBattleFieldStatic::select_your:
		for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
			if(*it!=m_pMainPlayer){
				m_CurPlayGuy = *it;
				break;
			}
		}
		break;
	default:
		RndSelectFirstPlayer();
		break;
	}

	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		if(*it==m_pMainPlayer){
			KDeckDefStatic* pDeckDef = KGameStaticMgr::getSingleton().GetDeckDef(pBattleStatic->GetMyDeck());
			(*it)->onBattleInit(m_CurPlayGuy==*it,pDeckDef,pBattleStatic->IsSelectCard());
		}else{
			int deckId = pQuest->IsDailyQuest()?pBattleStatic->GetYourDeck()+g_rnd.GetRandom(0,3):pBattleStatic->GetYourDeck();
			KDeckDefStatic* pDeckDef = KGameStaticMgr::getSingleton().GetDeckDef(deckId);

			if(m_sceneType==scene_tower){
				KDeckDefStatic* pTowerDeck = KTowerAssist::_createDeckDef(pDeckDef);
				(*it)->onBattleInit(m_CurPlayGuy==*it,pTowerDeck,pBattleStatic->IsSelectCard());
			}else{
				(*it)->onBattleInit(m_CurPlayGuy==*it,pDeckDef,pBattleStatic->IsSelectCard());
			}
		}
	}

	m_bFirstTurn = true;
	m_WaitTime = 0;
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_INIT,(unsigned long long)&m_BattleGuyList,(unsigned long long)m_world);
	if(pBattleStatic->IsSelectCard()){
		StateDelayJump(battle_select_handcard,1);
	}else{
		StateDelayJump(battle_turn_begin,2);
	}
	m_bDirty = true;
}

void KBattleCtrlBase::onCardSwitchOwner(KCardInst* pSrc,KCardInst* pDes)
{
	KBattleGuy* newOwner = pSrc->GetOwner();
	KBattleGuy* oldOwner = pDes->GetOwner();

	int slot = newOwner->GetDeck().GetRndEmptyFightSlot();
	pDes->onSwitchFightField(slot);
	oldOwner->GetDeck().RemoveCard(pDes);
	newOwner->GetDeck().updateCardSlot(pDes);
	newOwner->onCardEnterCtrl(this,pDes);
	oldOwner->onCardLeaveCtrl(this,pDes);
}

FBattleGuy* KBattleCtrlBase::GetCurPlayer()
{
	if(!m_CurPlayGuy) return NULL;
	return m_CurPlayGuy->GetFacade();
}

void KBattleCtrlBase::AddCardEvtAbility(KCardInst* card,KAbilityStatic::Enum_When when)
{
	m_cardWhenList.push_back(strDoCardWhenAbility(card,when));
}

void KBattleCtrlBase::DoCardEvtList(KCardInst* actor)
{
	int actorId = (actor)? actor->GetRealId():0;
	for(KDoCardWhenAbilityList::iterator it=m_cardWhenList.begin();it!=m_cardWhenList.end();++it){
		strDoCardWhenAbility& cardWhen = *it;
		KAbilityStatic* pAbility =  KSkillAssist::_findStaticAbility(cardWhen._card->GetCardId(),cardWhen._when);
		KBattleGod::getSingleton().DoCardAbility(this,pAbility,cardWhen._card,NULL,actorId);
	}

	m_cardWhenList.clear();
}

void KBattleCtrlBase::AddDramaElapsed(float val)
{ 
	m_waitDramaElapsed+=val;
}

KCardInst* KBattleCtrlBase::GetMyHero(int id)
{
	if(id<=0) return NULL;
	for(KBattleGuyList::iterator it = m_BattleGuyList.begin();it!=m_BattleGuyList.end();it++){
		KCardInst* pCard = (*it)->GetDeck().GetCard(id);
		if(pCard){
			return (*it)->GetDeck().GetHero();
		}
	}
	return NULL;
}

void KBattleCtrlBase::ForceWin()
{
	FBattleGuy* pEnemy = GetOtherPlayer();
	KBattleGuy* pGuy = pEnemy->GetImp();
	KCardInst* pHero = pGuy->GetDeck().GetHero();
	pHero->m_attr.setSlot(KCardInst::enum_slot_tomb);
	StateJump(battle_game_end);
}

void KBattleCtrlBase::ForceExit()
{
	KGameRecordMgr::getSingleton().Stop();
	KDynamicWorld::getSingleton().onBattleFailed(m_pBattleQuest->GetID(),m_sceneType);
	Clear();
}
