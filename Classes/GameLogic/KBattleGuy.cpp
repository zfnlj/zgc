#include "KBattleGuy.h"
#include "KBattleGod.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "System/Misc/KStream.h"
#include "assist/KSkillAssist.h"
#ifdef _USE_COCOS2DX
#include "../GameRecord/KGameRecordMgr.h"
#endif
#include "../PlayerCard/KPlayerCardDepot.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"

#define MAX_TURN_PLAY_TIME  30.0f //120.0f

KBattleGuy::KBattleGuy()
{

}
KBattleGuy* KBattleGuy::create(void)
{
	KBattleGuy* pGuy = KBattleGuy::Alloc();
	pGuy->init();
	return pGuy;
}

bool KBattleGuy::init(void)
{
	m_attr.clearAttrs();
	m_battleCtrl = NULL;
	m_TurnPlayTime = 0;
	m_bFirstHand = false;
	static int guyId = 1;
	m_guyId = guyId++;
	m_bSelectHandCard = false;
	m_Deck.initGuy(this);
	m_FacadeObj.SetImpl(this);
	return true;
}

void KBattleGuy::onBattleInit(bool bFirst,int deckId,bool bSelectCard)
{
	m_bFirstHand = bFirst;
	m_attr.clearAttrs();
	m_bSelectHandCard = bSelectCard;
	m_Deck.Clear();
	//m_heroSkillMgr.tmpInit(this);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DECKINIT,(unsigned long long)&m_FacadeObj,(unsigned long long)m_battleCtrl->GetWorld());

	KPlayerCardDepot* playerCardDepot = NULL;
	if(m_battleCtrl->GetMainPlayer()->GetImp()==this) playerCardDepot = KMainPlayer::RealPlayer()->GetCardDepot();
	KDeckDefStatic* pDeckDef = KGameStaticMgr::getSingleton().GetDeckDef(deckId);

	if(pDeckDef){
		m_Deck.initDeck(pDeckDef,bSelectCard);
		m_attr.setMaxRes(pDeckDef->getRes());
		m_attr.setCurRes(pDeckDef->getRes());
	}else if(m_Deck.createDeck(playerCardDepot)){
		m_attr.setMaxRes(1);
		m_attr.setCurRes(1);
		KHeroDef heroDef;
		playerCardDepot->PickCurHero(heroDef);
		m_Deck.m_heroSkillMgr.SetHero(&heroDef);
		m_Deck.DrawCard(bFirst?3:4,(bSelectCard)?KCardInst::enum_slot_select:KCardInst::enum_slot_hand);
	}else
	{
		m_Deck.createTestDeck();
		m_attr.setMaxRes(1);
		m_attr.setCurRes(1);
		if(bFirst){ //先手抓三张，后手抓四张
			m_Deck.DrawCard(3,(bSelectCard)?KCardInst::enum_slot_select:KCardInst::enum_slot_hand);
		}else{
			m_Deck.DrawCard(4,(bSelectCard)?KCardInst::enum_slot_select:KCardInst::enum_slot_hand);
		}
	}
}

void KBattleGuy::DoSelectBeginCard(KCardInstList* lst)
{
	m_bSelectHandCard = false;
	m_Deck.onSelectHandCardOK(lst);
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_SELECTCARD_OK,(unsigned long long)&m_FacadeObj,(unsigned long long)m_battleCtrl->GetWorld());
}

void KBattleGuy::onTurnEnd(KBattleCtrlBase* ctrl)
{
	m_Deck.OnTurnEnd(ctrl);
}

void KBattleGuy::onDrawCard()
{
	m_Deck.DrawCard(1);
}

void KBattleGuy::onTurnBegin(KBattleCtrlBase* ctrl,bool bFirstTurn)
{
	
	m_TurnPlayTime = 0;
	m_attr.setMaxRes(m_attr.getMaxRes()+1);
	m_attr.setCurRes(m_attr.getMaxRes());
	m_Deck.onTurnBegin(ctrl,bFirstTurn);

}


void KBattleGuy::onPlayCard(float dt,bool bOK)
{
#ifdef _USE_COCOS2DX
	if(KGameRecordMgr::getSingleton().IsPlaying()) dt=0.0f;
#endif
	if(bOK){
		m_TurnPlayTime = 0;
	}else{
		m_TurnPlayTime += dt;
	}
}

void KBattleGuy::ClearPlayTimeOut()
{
	m_TurnPlayTime = 0;
}

void KBattleGuy::SetPlayTimeOut()
{
	m_TurnPlayTime = MAX_TURN_PLAY_TIME+1.0f;
}
bool KBattleGuy::IsPlayTimeOut()
{

	return (m_TurnPlayTime >=MAX_TURN_PLAY_TIME);
}

float KBattleGuy::GetRemainPlayTime()
{
	return (MAX_TURN_PLAY_TIME-m_TurnPlayTime);
}

int  KBattleGuy::QuerySlotCardNum()
{
	return m_Deck.QueryCardSet(KCardInst::enum_slot_slot)->size();
}

KCardInstList* KBattleGuy::QueryCardSet(int slot)
{
	return m_Deck.QueryCardSet(slot);
}

void KBattleGuy::UseRes(int val)
{ 
	AddRes(-val);
	if(m_battleCtrl->IsServerSide()){
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_USERES,(unsigned long long)&m_FacadeObj,(unsigned long long)m_battleCtrl->GetWorld());	
	}
}

void KBattleGuy::onGameEnd()
{
	m_Deck.Clear();
}

void KBattleGuy::update(float dt)
{
	m_Deck.UpdateActor(dt);
}

void KBattleGuy::Clear()
{
	m_Deck.Clear();
}

void KBattleGuy::QueryResInfo(char* buf)
{
	sprintf(buf,"%d/%d",m_attr.getCurRes(),m_attr.getMaxRes());
}

void KBattleGuy::QueryValidateHandCards(KCardInstList* lst)
{
	m_Deck.QueryValidateHandCards(lst,m_attr.getCurRes());
}

void KBattleGuy::QueryValidateFightCards(KCardInstList* lst)
{
	m_Deck.QueryValidateFightCards(lst,m_attr.getCurRes());
}

void KBattleGuy::QuerySleepFightCards(KCardInstList* lst)
{
	m_Deck.QuerySleepFightCards(lst);
}

void KBattleGuy::QueryActiveDefendCards(KCardInstList* lst)
{
	m_Deck.QueryActiveDefendCards(lst);
}

size_t KBattleGuy::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteUint64(m_guyId)) return 0;
	if(!m_attr.writePacketAll(so,true)) return 0;


	if(!m_Deck.serialize(so))
		return 0;
	return so->size() - pos;
}

size_t KBattleGuy::serializeDirty(KMemoryStream* so)
{
	size_t pos = so->size();
	m_attr.writePacket(so);
	m_Deck.serializeDirty(so);
	return so->size() - pos;
}

BOOL KBattleGuy::deserializeDirty(KMemoryStream* si)
{
	m_attr.readPacket(si);
	if(!m_Deck.deserializeDirty(si))
		return FALSE;
	return TRUE;
}

BOOL KBattleGuy::deserialize(KMemoryStream* si)
{
	if(!si->ReadUint64(m_guyId))
		return FALSE;
	m_attr.readPacket(si);
	if(!m_Deck.deserialize(si))
		return FALSE;
	return TRUE;
}

void KBattleGuy::AddRes(int val)
{ 
	m_attr.setCurRes(m_attr.getCurRes()+val);
}

void KBattleGuy::onCardLeaveCtrl(KBattleCtrlBase* ctrl,KCardInst* card)
{
	KCardBufferList::iterator it =card->m_attr.m_bufList.begin();
	while(it != card->m_attr.m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhich()==KAbilityStatic::which_owner) RemoveGuyAbility(buf._pST);
		it++;
	}
}

void KBattleGuy::onCardEnterCtrl(KBattleCtrlBase* ctrl,KCardInst* card)
{
	KAbilityStatic* pAbility =KSkillAssist::_findStaticAbility(card->GetCardId(),KAbilityStatic::when_enter);
	DoGuyAbility(ctrl,card,pAbility,0);
}

void KBattleGuy::AddRes(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility)
{
	AddRes(pAbility->GetNormalVal());
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_UPDATEINFO,0,(unsigned long long)ctrl->GetWorld());
}

bool KBattleGuy::DoGuyAbility(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor)
{
	if(actor==0) actor = pSrc->GetRealId();
	if(!pAbility) return false;
	if(pAbility->GetWhich()!=KAbilityStatic::which_owner) return false;
	switch(pAbility->GetWhat()){
	case KAbilityStatic::what_summon:
		{
			KSkillAssist::_summonCard(ctrl,pSrc,pAbility,actor);
		}
		break;
	case KAbilityStatic::what_copy_fight:
		{
			if(pSrc->GetOwner()->GetDeck().GetEmptyFightSlot()<0) return false;
			KSkillAssist::_copyFightSoldier(ctrl,pSrc,pAbility,actor);
		}
		break;
	case KAbilityStatic::what_get_card:
		{
			KSkillAssist::_abilityGenCard(ctrl,&m_Deck,pSrc,pAbility,actor);
		}
		break;
	case KAbilityStatic::what_copy_hand:
		{
			KSkillAssist::_copyHandCard(ctrl,pSrc,pAbility,actor);
		}
		break;
	case KAbilityStatic::what_clear_hand:
		{
			KSkillAssist::_clearHandCard(ctrl,pSrc,pAbility,actor);
		}
		break;
	case KAbilityStatic::what_res_add:
		{
			AddRes(pAbility->GetNormalVal());
			strCardAbilityResult result;
			result.init(actor,pSrc->GetRealId(),pAbility);
			KSkillAssist::_sendAbilityResult(ctrl,result);
		}
		break;
	case KAbilityStatic::what_draw_card:
		{
			strCardAbilityResult result;
			result.init(actor,pSrc->GetRealId(),pAbility);

			m_Deck.DrawCard( KSkillAssist::_calcValDef(ctrl,this,pSrc,pAbility->GetVal()),KCardInst::enum_slot_hand,&result);
			KSkillAssist::_sendAbilityResult(ctrl,result);
		}
		break;
	default:
		m_attr.AddBuf(pAbility);
		break;
	}
	if(!pAbility->BufIconEmpty()) pSrc->AddBuf(pAbility);
	return true;
}

void KBattleGuy::RemoveGuyAbility(KAbilityStatic* pAbility)
{
	m_attr.RemoveBuf(pAbility->GetId());
}

int KBattleGuy::calcHurtVal(int val)
{
	int realVal = val;
	KAbilityStatic* pBuf = KSkillAssist::_findBuf(m_attr.m_bufList,KAbilityStatic::what_sp_rate);
	if(pBuf){
		realVal = val*pBuf->GetNormalVal();
	}

	KCardAbilityList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		if((*it)->GetWhat()== KAbilityStatic::what_cast_add) realVal += (*it)->GetNormalVal();
		it++;
	}
	return realVal;
}

int KBattleGuy::calcHealVal(int val)
{
	KAbilityStatic* pBuf = KSkillAssist::_findBuf(m_attr.m_bufList,KAbilityStatic::what_sp_rate);
	if(pBuf){
		return val*pBuf->GetNormalVal();
	}else{
		return val;
	}
}

int KBattleGuy::calcMpCost(int val)
{
	KCardAbilityList::iterator it = m_attr.m_bufList.begin();
	while(it != m_attr.m_bufList.end()){
		if((*it)->GetWhat()== KAbilityStatic::what_mp_cost) val += (*it)->GetNormalVal();
		it++;
	}
	return val;
}

int KBattleGuy::GetFighterNum()
{
	KCardInstList* fighterLst = m_Deck.QueryCardSet(KCardInst::enum_slot_fight);
	return fighterLst->size();
}