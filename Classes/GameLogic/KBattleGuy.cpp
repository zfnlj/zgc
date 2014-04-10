#include "KBattleGuy.h"
#include "KBattleGod.h"
#include "KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "System/Misc/KStream.h"
#include "assist/KSkillAssist.h"

#define MAX_TURN_PLAY_TIME 2*60

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
	m_bSelectHandCard = true;
	m_Deck.Clear();

	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_BATTLE_DECKINIT,(unsigned long long)&m_FacadeObj,(unsigned long long)m_battleCtrl->GetWorld());
	KDeckDefStatic* pDeckDef = KGameStaticMgr::getSingleton().GetDeckDef(deckId);
	if(pDeckDef){
		m_Deck.initDeck(pDeckDef);
		m_Deck.createCards();
		m_Deck.GetHero()->HpSet(pDeckDef->getHeroHp());
		m_attr.setMaxRes(pDeckDef->getRes());
		m_attr.setCurRes(pDeckDef->getRes());
		m_Deck.DrawCard(pDeckDef->getDrawNum(),(bSelectCard)?KCardInst::enum_slot_select:KCardInst::enum_slot_hand);
	}else{
		m_Deck.createCards();
		m_attr.setMaxRes(8);
		m_attr.setCurRes(8);
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

void KBattleGuy::onTurnBegin(KBattleCtrlBase* ctrl,bool bFirstTurn)
{
	if(!bFirstTurn){
		m_Deck.DrawCard(1);
	}
	m_TurnPlayTime = 0;
	m_attr.setMaxRes(m_attr.getMaxRes()+1);
	m_attr.setCurRes(m_attr.getMaxRes());
	m_Deck.onTurnBegin(ctrl);
}


void KBattleGuy::onPlayCard(float dt,bool bOK)
{
	if(bOK){
		m_TurnPlayTime = 0;
	}else{
		m_TurnPlayTime += dt;
	}
}

void KBattleGuy::SetPlayTimeOut()
{
	m_TurnPlayTime = MAX_TURN_PLAY_TIME;
}
bool KBattleGuy::IsPlayTimeOut()
{
	return (m_TurnPlayTime >MAX_TURN_PLAY_TIME);
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
	sprintf(buf,"%d",m_attr.getCurRes());
}

void KBattleGuy::QueryActiveHandCards(KCardInstList* lst)
{
	m_Deck.QueryActiveHandCards(lst,m_attr.getCurRes());
}

void KBattleGuy::QueryActiveFightCards(KCardInstList* lst)
{
	m_Deck.QueryActiveFightCards(lst,m_attr.getCurRes());
}

void KBattleGuy::QueryActiveDefendCards(KCardInstList* lst)
{
	m_Deck.QueryActiveDefendCards(lst);
}

size_t KBattleGuy::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteUint64(m_guyId))
		return 0;
	m_attr.writePacketAll(so,true);

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

void KBattleGuy::onCardLeaveCtrl(KCardInst* card)
{
	KAbilityStatic* pAbility = card->FindBuf(KAbilityStatic::when_dead);
	DoGuyAbility(pAbility);
}

void KBattleGuy::onCardEnterCtrl(KCardInst* card)
{
	KAbilityStatic* pAbility = card->FindBuf(KAbilityStatic::when_enter);
	DoGuyAbility(pAbility);
}

bool KBattleGuy::DoGuyAbility(KAbilityStatic* pAbility)
{
	if(!pAbility) return false;
	if(pAbility->GetWhich()!=KAbilityStatic::which_owner) return false;
	if(pAbility->GetWhat()==KAbilityStatic::what_remove_buf){
		KSkillAssist::_removeBuf(m_bufList,pAbility->GetVal());
	}else{
		m_bufList.push_back(pAbility);
	}
	return true;
}

int KBattleGuy::calcHurtVal(int val)
{
	KAbilityStatic* pBuf = KSkillAssist::_findBuf(m_bufList,KAbilityStatic::what_sp_rate);
	if(pBuf){
		return val*pBuf->GetVal();
	}else{
		return val;
	}
}

int KBattleGuy::calcHealVal(int val)
{
	KAbilityStatic* pBuf = KSkillAssist::_findBuf(m_bufList,KAbilityStatic::what_sp_rate);
	if(pBuf){
		return val*pBuf->GetVal();
	}else{
		return val;
	}
}