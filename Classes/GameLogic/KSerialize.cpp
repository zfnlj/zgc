#include "KSerialize.h"
#include "KBattleGuy.h"
#include "KBattleCtrlBase.h"
#include "assist/KBattleCtrlAssist.h"

void KSerialize::deserializeDirty(KBattleCtrlBase* ctrl,UINT64 guyId,KMemoryStream* si)
{
	KBattleGuy* guy = KBattleCtrlAssist::GetGuy(ctrl,guyId);
	if(!guy) return;
	deserializeDirty(guy,si);
}

void KSerialize::deserializeBattleGuy(KBattleCtrlBase* ctrl,UINT64 guyId,KMemoryStream* si,bool bMainPlayer)
{
	KBattleGuy* guy = KBattleCtrlAssist::GetGuy(ctrl,guyId);
	if(!guy){
		guy = KBattleGuy::create();
		ctrl->m_BattleGuyList.push_back(guy);
	}
	deserialize(guy,si);
	guy->SetBattleCtrl(ctrl);
	if(bMainPlayer) ctrl->m_pMainPlayer = guy;
}


size_t KSerialize::serialize(KBattleCtrlBase* ctrl,KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteByte((BYTE)ctrl->m_state))
		return 0;
	UINT64 playId = (ctrl->m_CurPlayGuy)? ctrl->m_CurPlayGuy->GetGuyId():0;
	if(!so->WriteUint64(playId))
		return 0;
	return so->size() - pos;

}

bool KSerialize::deserialize(KBattleCtrlBase* ctrl,KMemoryStream* si)
{
	BYTE statVal = 0;
	if(!si->ReadByte(statVal))
		return false;
	UINT64 playId;
	if(!si->ReadUint64(playId))
		return false;
	KBattleGuy* guy = KBattleCtrlAssist::GetGuy(ctrl,playId);
	ctrl->m_CurPlayGuy = guy;
	ctrl->m_state = (KBattleCtrlBase::BattleState)statVal;
	return true;
}

size_t KSerialize::serializeAll(KBattleCtrlBase* ctrl,KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteByte((BYTE)ctrl->m_state)) return 0;
	if(ctrl->m_state==KBattleCtrlBase::battle_null) return so->size() - pos;

	if(!so->WriteUint64(ctrl->m_pMainPlayer->GetGuyId())) return 0;
	if(!so->WriteUint64(ctrl->m_CurPlayGuy->GetGuyId())) return 0;
	if(!so->WriteByte((BYTE)ctrl->m_BattleGuyList.size())) return 0;
	for(KBattleGuyList::iterator it = ctrl->m_BattleGuyList.begin();it!=ctrl->m_BattleGuyList.end();it++){
		serialize((*it),so);
	}
	return so->size() - pos;

}

bool KSerialize::deserializeAll(KBattleCtrlBase* ctrl,KMemoryStream* si)
{
	ctrl->Clear();
	BYTE statVal = 0;
	if(!si->ReadByte(statVal))
		return false;
	if(statVal==(int)KBattleCtrlBase::battle_null) return true;

	UINT64 mainPlayerId,curPlayerId;
	if(!si->ReadUint64(mainPlayerId))
		return false;
	if(!si->ReadUint64(curPlayerId))
		return false;
	BYTE guyNum;
	if(!si->ReadByte(guyNum))
		return false;
	for(int i=0;i<guyNum;i++){
		KBattleGuy* guy = KBattleGuy::create();
		ctrl->m_BattleGuyList.push_back(guy);
		guy->SetBattleCtrl(ctrl);
		
	}
	for(KBattleGuyList::iterator it = ctrl->m_BattleGuyList.begin();it!=ctrl->m_BattleGuyList.end();it++){
		deserialize((*it),si);
		if((*it)->GetGuyId()==mainPlayerId) ctrl->m_pMainPlayer = (*it);
		if((*it)->GetGuyId()==curPlayerId) ctrl->m_CurPlayGuy = (*it);
	}
	ctrl->m_state = (KBattleCtrlBase::BattleState)statVal;
	return true;
}


/////////////// deck


size_t KSerialize::serialize(KBattleDeck* deck,KMemoryStream* so)
{
	size_t pos = so->size();
	if(!deck->m_heroSkillMgr.serialize(so)) return 0;
	if(!serializeCardList(deck,so,deck->m_HeroCardSet))
		return 0;
	if(!serializeCardList(deck,so,deck->m_HandCardSet,true))
		return 0;
	if(!serializeCardList(deck,so,deck->m_FightCardSet))
		return 0;
	if(!serializeCardList(deck,so,deck->m_EquipCardSet))
		return 0;
	if(!serializeCardList(deck,so,deck->m_SlotCardSet,true))
		return 0;
	if(!serializeCardList(deck,so,deck->m_TombCardSet,true))
		return 0;
    if(!serializeCardList(deck,so,deck->m_SecretCardSet))
		return 0;
	return so->size() - pos;

}

size_t KSerialize::serializeCardList(KBattleDeck* deck,KMemoryStream* so,KCardInstList& lst,bool newCard)
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

BOOL KSerialize::deserialize(KBattleDeck* deck,KMemoryStream* si)
{
	if(!deck->m_heroSkillMgr.deserialize(si)) return FALSE;
	if(!deserializeCardList(deck,si,deck->m_HeroCardSet))
		return FALSE;
	if(!deserializeCardList(deck,si,deck->m_HandCardSet))
		return FALSE;
	if(!deserializeCardList(deck,si,deck->m_FightCardSet))
		return FALSE;
	if(!deserializeCardList(deck,si,deck->m_EquipCardSet))
		return FALSE;
	if(!deserializeCardList(deck,si,deck->m_SlotCardSet))
		return FALSE;
	if(!deserializeCardList(deck,si,deck->m_TombCardSet))
		return FALSE;
    if(!deserializeCardList(deck,si,deck->m_SecretCardSet))
		return FALSE;
	return TRUE;
}

BOOL KSerialize::deserializeCardList(KBattleDeck* deck,KMemoryStream* si,KCardInstList& lst)
{
	_clearCardList(&lst);
	BYTE n;
	if(!si->ReadByte(n))
		return FALSE;
	for(int i=0;i<n;i++){
		KCardInst* card = KCardInst::create();
		if(!card->deserialize(si)) return FALSE;
		card->SetOwner(deck->m_Owner);
		lst.push_back(card);
	}
	return TRUE;
}

size_t KSerialize::serializeDirty(KBattleDeck* deck,KMemoryStream* so)
{
	size_t pos = so->size();
	KCardInstList tmpList;
	_fillDirtyCardList(deck->m_HeroCardSet,tmpList);
	_fillDirtyCardList(deck->m_HandCardSet,tmpList);
	_fillDirtyCardList(deck->m_FightCardSet,tmpList);
	_fillDirtyCardList(deck->m_EquipCardSet,tmpList);
	_fillDirtyCardList(deck->m_TombCardSet,tmpList);
    _fillDirtyCardList(deck->m_SecretCardSet,tmpList);
	unsigned char n = (unsigned char)tmpList.size();
	so->WriteByte(n);
	for(KCardInstList::iterator it=tmpList.begin();it!=tmpList.end();++it){
		KCardInst* card = *it;
		if(!card->serializeDirty(so)) return -1;
	}
	return so->size() - pos;
}

BOOL KSerialize::deserializeDirty(KBattleDeck* deck,KMemoryStream* si)
{
	unsigned char n;
	if(!si->ReadByte(n))
		return FALSE;
	for(int i=0;i<n;i++){
		int realId;
		if(!si->ReadInt(realId)) return FALSE;
		KCardInst* card = deck->GetCard(realId);
		//CCAssert(card , "deserialize error--Not find the card!");
		if(card){
			card->deserialize(si);
			deck->updateCardSlot(card);
		}else{
			card = KCardInst::create();
			card->deserialize(si);
			KCardInstList* cardSet = deck->QueryCardSet(card->GetSlot());
			if(cardSet) cardSet->push_back(card);
		}
	}
	return TRUE;
}


// BattleGuy


size_t KSerialize::serialize(KBattleGuy* guy,KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteUint64(guy->m_guyId)) return 0;
	if(!guy->m_attr.writePacketAll(so,true)) return 0;


	if(!KSerialize::serialize(&guy->m_Deck,so))
		return 0;
	return so->size() - pos;
}

size_t KSerialize::serializeDirty(KBattleGuy* guy,KMemoryStream* so)
{
	size_t pos = so->size();
	guy->m_attr.writePacket(so);
	KSerialize::serializeDirty(&guy->m_Deck,so);
	return so->size() - pos;
}

BOOL KSerialize::deserializeDirty(KBattleGuy* guy,KMemoryStream* si)
{
	guy->m_attr.readPacket(si);
	if(!KSerialize::deserializeDirty(&guy->m_Deck,si))
		return FALSE;
	return TRUE;
}

BOOL KSerialize::deserialize(KBattleGuy* guy,KMemoryStream* si)
{
	if(!si->ReadUint64(guy->m_guyId))
		return FALSE;
	guy->m_attr.readPacket(si);
	if(!KSerialize::deserialize(&guy->m_Deck,si))
		return FALSE;
	return TRUE;
}
